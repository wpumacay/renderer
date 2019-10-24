
#include <app/CApplication.h>

namespace engine
{
    CApplication* CApplication::s_instance = nullptr;

    CApplication* CApplication::GetInstance()
    {
        ENGINE_CORE_ASSERT( CApplication::s_instance, "Must have a valid application-instance created" );

        return CApplication::s_instance;
    }

    CApplication::CApplication()
    {
        // create the global reference for other systems to use
        CApplication::s_instance = this;

        engine::CLogger::Init();
        ENGINE_CORE_INFO( "GL-Application: starting..." );

        CWindowProps _windowProperties;
        _windowProperties.width = 800;
        _windowProperties.height = 600;
        _windowProperties.title = "Minino-engine :3";
        _windowProperties.callbackKey = nullptr;
        _windowProperties.callbackMouse = nullptr;
        _windowProperties.callbackMouseMove = nullptr;

        m_window = std::unique_ptr< COpenGLWindow >( new COpenGLWindow( _windowProperties ) );

        ENGINE_CORE_ASSERT( m_window->glfwWindow(), "There was an error while creating the opengl-window" );

        engine::CTime::Init();
        engine::CTextureManager::Init();
        engine::CShaderManager::Init();
        engine::CInputManager::Init();
        engine::CDebugDrawer::Init();
        engine::CNoiseGenerator::Init();

        m_scene         = std::unique_ptr< CScene >( new CScene() );
        m_mainRenderer  = std::unique_ptr< CMainRenderer >( new CMainRenderer() );
        m_imguiManager  = std::unique_ptr< CImGuiManager >( new CImGuiManager( m_window->glfwWindow() ) );

        m_window->registerKeyCallback( CApplication::CallbackKey );
        m_window->registerMouseCallback( CApplication::CallbackMouseButton );
        m_window->registerMouseMoveCallback( CApplication::CallbackMouseMove );
        m_window->registerScrollCallback( CApplication::CallbackScroll );
        m_window->registerResizeCallback( CApplication::CallbackResize );

        // start keeping track of time
        engine::CTime::Start();
        m_timeStamp = 0.0f;

        // create some default resources for our renderer to use

        ENGINE_CORE_INFO( "GL-Application started successfully!!!" );
    }

    CApplication::~CApplication()
    {
        m_scene         = nullptr;
        m_mainRenderer  = nullptr;
        m_imguiManager  = nullptr;
        m_window        = nullptr;

        engine::CTime::Release();
        engine::CDebugDrawer::Release();
        engine::CInputManager::Release();
        engine::CShaderManager::Release();
        engine::CTextureManager::Release();
        engine::CNoiseGenerator::Release();
    }

    void CApplication::setScene( std::unique_ptr< CScene > scene )
    {
        m_scene = std::move( scene );
    }

    void CApplication::addGuiLayer( std::unique_ptr< CImGuiLayer > layer )
    {
        m_guiLayers.push_back( std::move( layer ) );
    }

    void CApplication::update()
    {
        // update scene resources (cameras, ...)
        if ( m_scene )
            m_scene->update();

        // update all gui-layers
        for ( auto& layer : m_guiLayers )
            layer->update();
    }

    void CApplication::begin()
    {
        m_timeStamp = glfwGetTime();
        // prepare window for rendering, and poll events
        m_window->begin();

        // configure some resources (if available) required for rendering
        if ( m_scene && m_scene->currentCamera() )
            m_renderOptions.cameraPtr = m_scene->currentCamera();

        if ( m_scene && m_scene->mainLight() )
            m_renderOptions.lightPtr = m_scene->mainLight();

        if ( m_scene && m_scene->skybox() )
            m_renderOptions.skyboxPtr = m_scene->skybox();

        if ( m_scene && m_scene->fog() )
            m_renderOptions.fogPtr = m_scene->fog();

        if ( m_scene && m_mainRenderer && m_mainRenderer->shadowMap() )
            m_renderOptions.shadowMapPtr = m_mainRenderer->shadowMap();

        // prepare our renderer to accepts rendering commands
        m_mainRenderer->begin( m_renderOptions );

        // prepare imgui prior to accepts any rendering commands
        m_imguiManager->begin();
    }

    void CApplication::render()
    {
        ENGINE_CORE_ASSERT( m_scene, "There must be a valid scene for this application" );

        // submit the renderables of this scene to the renderer
        m_mainRenderer->submit( m_scene->renderables() );

        // render all commands requested to our renderer
        m_mainRenderer->render();

        // render all gui-layers
        for ( auto& layer : m_guiLayers )
            layer->render();

        // render all commands requested to the debug drawer
        auto _camera = m_scene->currentCamera();
        auto _light = m_scene->mainLight();
        if ( _camera && _light )
            CDebugDrawer::Render( _camera, _light );
        else if ( _camera )
            CDebugDrawer::Render( _camera );

        // render all commands requested to imgui so far
        m_imguiManager->render();
    }

    void CApplication::end()
    {
        // swap buffers such that we see that juicy frame (if we rendered to default target)
        m_window->end();

        // tick-tock, in the main thread (might need to handle it differently in multi-threading mode)
        auto _timeNow = glfwGetTime();
        auto _timeDelta = _timeNow - m_timeStamp;
        m_timeStamp = _timeNow;

        // update time keeper for our other systems to use
        engine::CTime::Update( _timeDelta );
    }

    void CApplication::CallbackKey( int key, int action )
    {
        auto _app = CApplication::GetInstance();
        auto& _layers = _app->m_guiLayers;
        auto _name = std::string( "key_event: " ) + std::to_string( key ) + " - " + std::to_string( action );

        if ( action == ENGINE_KEY_PRESSED ) 
        {
            auto _ev = CKeyPressedEvent( _name, key );
            //// ENGINE_CORE_TRACE( "event information:\n\r{0}", engine::toString( _ev ) );

            for ( auto& _layer : _layers )
                if ( _layer->onEvent( _ev ) )
                    return;
        }
        else if ( action == ENGINE_KEY_RELEASED ) 
        {
            auto _ev = CKeyReleasedEvent( _name, key );
            //// ENGINE_CORE_TRACE( "event information:\n\r{0}", engine::toString( _ev ) );

            for ( auto& _layer : _layers )
                if ( _layer->onEvent( _ev ) )
                    return;
        }
        else if ( action == ENGINE_KEY_REPEAT ) 
        {
            auto _ev = CKeyPressedEvent( _name, key );
            //// ENGINE_CORE_TRACE( "event information:\n\r{0}", engine::toString( _ev ) );

            for ( auto& _layer : _layers )
                if ( _layer->onEvent( _ev ) )
                    return;
        }

        CInputManager::Callback_key( key, action );
    }

    void CApplication::CallbackMouseButton( int button, int action, double x, double y )
    {
        auto _app = CApplication::GetInstance();
        auto& _layers = _app->m_guiLayers;
        auto _name = std::string( "mouse_button_event: " ) + std::to_string( button ) + " - " + 
                            std::to_string( action ) + " - " + std::to_string( x ) + " - " + std::to_string( y );

        bool _handled = false;
        if ( action == ENGINE_MOUSE_BUTTON_PRESSED ) 
        {
            auto _ev = CMousePressedEvent( _name, button, { (float)x, (float)y } );
            //// ENGINE_CORE_TRACE( "event information:\n\r{0}", engine::toString( _ev ) );

            for ( auto& _layer : _layers )
            {
                _handled = _layer->onEvent( _ev );
                if ( _handled )
                    break;
            }
        }
        else if ( action == ENGINE_MOUSE_BUTTON_RELEASED ) 
        {
            auto _ev = CMouseReleasedEvent( _name, button, { (float)x, (float)y } );
            //// ENGINE_CORE_TRACE( "event information:\n\r{0}", engine::toString( _ev ) );

            for ( auto& _layer : _layers )
            {
                _handled = _layer->onEvent( _ev );
                if ( _handled )
                    break;
            }
        };

        if ( _handled ) // clear the state for this button
            CInputManager::Callback_mouse( button, ENGINE_MOUSE_BUTTON_RELEASED, x, y );
        else
            CInputManager::Callback_mouse( button, action, x, y );
    }

    void CApplication::CallbackMouseMove( double x, double y )
    {
        auto _app = CApplication::GetInstance();
        auto& _layers = _app->m_guiLayers;
        auto _name = std::string( "mouse_move_event: " ) + std::to_string( x ) + " - " + std::to_string( y );

        auto _ev = CMouseMoveEvent( _name, -1, { (float)x, (float)y } );
        //// ENGINE_CORE_TRACE( "event information:\n\r{0}", engine::toString( _ev ) );

        for ( auto& _layer : _layers )
            if ( _layer->onEvent( _ev ) )
                return;

        CInputManager::Callback_mouseMove( x, y );
    }

    void CApplication::CallbackScroll( double xOff, double yOff )
    {
        auto _app = CApplication::GetInstance();
        auto& _layers = _app->m_guiLayers;
        auto _name = std::string( "mouse_scroll_event: " ) + std::to_string( xOff ) + " - " + std::to_string( yOff );

        auto _ev = CMouseScrollEvent( _name, { (float)xOff, (float)yOff } );
        //// ENGINE_CORE_TRACE( "event information:\n\r{0}", engine::toString( _ev ) );

        for ( auto& _layer : _layers )
            if ( _layer->onEvent( _ev ) )
                return;

        CInputManager::Callback_scroll( xOff, yOff );
    }

    void CApplication::CallbackResize( int width, int height )
    {
        auto _app = CApplication::GetInstance();
        auto& _scene = _app->m_scene;
        auto& _layers = _app->m_guiLayers;
        auto _name = std::string( "resize_event: " ) + std::to_string( width ) + " - " + std::to_string( height );

        auto _ev = CResizeEvent( _name, width, height );
        //// ENGINE_CORE_TRACE( "event information:\n\r{0}", engine::toString( _ev ) );

        for ( auto& _layer : _layers )
            _layer->onEvent( _ev );

        if ( _scene )
            _scene->resize( width, height );
    }

    std::vector< CImGuiLayer* > CApplication::guiLayers() const
    {
        std::vector< CImGuiLayer* > _layersPtrs;
        for ( auto& layer : m_guiLayers )
            _layersPtrs.push_back( layer.get() );

        return _layersPtrs;
    }
}
