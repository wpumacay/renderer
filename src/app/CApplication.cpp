
#include <app/CApplication.h>

namespace engine
{
    CApplication* CApplication::s_instance = nullptr;

    CApplication* CApplication::GetInstance()
    {
        ENGINE_CORE_ASSERT( CApplication::s_instance, "Must have a valid application-instance created" );

        return CApplication::s_instance;
    }

    CApplication::CApplication( const CWindowProps& windowProperties,
                                const CImGuiProps& imguiProperties )
    {
        // create the global reference for other systems to use
        CApplication::s_instance = this;

        engine::CLogger::Init();
        ENGINE_CORE_INFO( "GL-Application: starting..." );

        m_window = std::unique_ptr< COpenGLWindow >( new COpenGLWindow( windowProperties ) );

        ENGINE_CORE_ASSERT( m_window->glfwWindow(), "There was an error while creating the opengl-window" );

        //// engine::CProfilingManager::Init( eProfilerType::INTERNAL );
        engine::CProfilingManager::Init( eProfilerType::EXTERNAL_CHROME );
        ENGINE_CORE_INFO( "GL-Application: profiling enabled" );
        engine::CProfilingManager::BeginSession( "sess_core_init" );
        PROFILE_FUNCTION_IN_SESSION( "sess_core_init" );

        engine::CTime::Init();
        engine::CTextureManager::Init();
        engine::CShaderManager::Init();
        engine::CInputManager::Init();
        engine::CDebugDrawer::Init();
        engine::CNoiseGenerator::Init();

        //// PROFILE_SCOPE_IN_SESSION( "app_initialization", "sess_core_init" );
        m_scene         = std::unique_ptr< CScene >( new CScene() );
        m_mainRenderer  = std::unique_ptr< CMainRenderer >( new CMainRenderer() );
        m_imguiManager  = std::unique_ptr< CImGuiManager >( new CImGuiManager( m_window->glfwWindow(), imguiProperties ) );
        m_renderTarget  = std::unique_ptr< CFrameBuffer >( _createRenderTarget() );

        m_window->registerKeyCallback( &CApplication::CallbackKey );
        m_window->registerMouseCallback( &CApplication::CallbackMouseButton );
        m_window->registerMouseMoveCallback( &CApplication::CallbackMouseMove );
        m_window->registerScrollCallback( &CApplication::CallbackScroll );
        m_window->registerResizeCallback( &CApplication::CallbackResize );

        // setup initial viewport
        m_renderOptions.viewportWidth = m_window->width();
        m_renderOptions.viewportHeight = m_window->height();

        // start keeping track of time
        engine::CTime::Start();
        m_timeStamp = 0.0f;

        // create a utils panel by default (the user can choose to use it or not)
        m_guiUtilsLayer = new CImGuiUtilsLayer( "Utils-layer",
                                                m_scene.get(),
                                                m_mainRenderer.get(),
                                                &m_renderOptions,
                                                m_imguiManager.get(),
                                                m_window.get() );

        m_guiUtilsLayer->setActive( false );
        addGuiLayer( std::unique_ptr< CImGuiLayer >( m_guiUtilsLayer ) );

        ENGINE_CORE_INFO( "GL-Application started successfully!!!" );
    }

    CApplication::~CApplication()
    {
        m_scene         = nullptr;
        m_mainRenderer  = nullptr;
        m_imguiManager  = nullptr;
        m_window        = nullptr;

        //// // flush results from initialization
        //// engine::CProfilingManager::EndSession( "sess_core_init" );

        engine::CTime::Release();
        engine::CProfilingManager::Release();
        engine::CDebugDrawer::Release();
        engine::CInputManager::Release();
        engine::CShaderManager::Release();
        engine::CTextureManager::Release();
        engine::CNoiseGenerator::Release();
    }

    void CApplication::setScene( std::unique_ptr< CScene > scene )
    {
        m_scene = std::move( scene );
        // update resources that require the scene reference
        m_guiUtilsLayer->setScene( m_scene.get() );
    }

    void CApplication::setOffscreenRendering( bool enabled )
    {
        m_useRenderTarget = enabled;
        m_renderOptions.renderTargetPtr = ( enabled ) ? m_renderTarget.get() : nullptr;
    }

    void CApplication::setGuiActive( bool enabled )
    {
        m_imguiManager->setActive( enabled );
    }

    void CApplication::setGuiUtilsActive( bool enabled )
    {
        m_guiUtilsLayer->setActive( enabled );
    }

    void CApplication::setGuiSceneViewActive( bool enabled )
    {
        // @todo: activate scene-view and enable render-target to use it for later visualization
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
        if ( m_imguiManager->active() )
            for ( auto& layer : m_guiLayers )
                if ( layer->active() )
                    layer->update();
    }

    void CApplication::begin()
    {
        engine::CProfilingManager::BeginSession( "sess_core_render" );

        m_timeStamp = glfwGetTime();
        // prepare window for rendering, and poll events
        m_window->begin();

        // configure some resources (if available) required for rendering
        if ( m_scene && m_scene->currentCamera() )
            m_renderOptions.cameraPtr = m_scene->currentCamera();

        if ( m_scene && m_scene->mainLight() )
        {
            auto _light = m_scene->mainLight();
            auto _lightType = _light->type();

            // use this main light for rendering purposes
            m_renderOptions.lightPtr = _light;

            // configure shadow-range options according to the type of light we have
            if ( _lightType == eLightType::POINT || _lightType == eLightType::SPOT )
                m_renderOptions.shadowMapRangeConfig.type = eShadowRangeType::FIXED_USER;
            m_renderOptions.shadowMapRangeConfig.dirLightPtr = nullptr;
            m_renderOptions.shadowMapRangeConfig.pointLightPtr = nullptr;
            m_renderOptions.shadowMapRangeConfig.spotLightPtr = nullptr;

            if ( _lightType == eLightType::DIRECTIONAL )
                m_renderOptions.shadowMapRangeConfig.dirLightPtr = dynamic_cast< CDirectionalLight* >( _light );
            else if ( _lightType == eLightType::POINT )
                m_renderOptions.shadowMapRangeConfig.pointLightPtr = dynamic_cast< CPointLight* >( _light );
            else if ( _lightType == eLightType::SPOT )
                m_renderOptions.shadowMapRangeConfig.spotLightPtr = dynamic_cast< CSpotLight* >( _light );
        }

        if ( m_scene && m_scene->skybox() )
            m_renderOptions.skyboxPtr = m_scene->skybox();

        if ( m_scene && m_scene->fog() )
            m_renderOptions.fogPtr = m_scene->fog();

        if ( m_scene && m_mainRenderer && m_mainRenderer->shadowMap() )
            m_renderOptions.shadowMapPtr = m_mainRenderer->shadowMap();

        // prepare our renderer to accepts rendering commands
        m_mainRenderer->begin( m_renderOptions );
    }

    void CApplication::render()
    {
        ENGINE_CORE_ASSERT( m_scene, "There must be a valid scene for this application" );
        PROFILE_FUNCTION_IN_SESSION( "sess_core_render" );

        // submit the renderables of this scene to the renderer
        m_mainRenderer->submit( m_scene->renderables() );

        // render all commands requested to our renderer
        m_mainRenderer->render();

        // prepare imgui prior to accepts any rendering commands
        m_imguiManager->begin();

        // render all gui-layers
        if ( m_imguiManager->active() )
            for ( auto& layer : m_guiLayers )
                if ( layer->active() )
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

        engine::CProfilingManager::EndSession( "sess_core_render" );
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
            {
                if ( !_layer->active() )
                    continue;

                if ( _layer->onEvent( _ev ) )
                    return;
            }
        }
        else if ( action == ENGINE_KEY_RELEASED ) 
        {
            auto _ev = CKeyReleasedEvent( _name, key );
            //// ENGINE_CORE_TRACE( "event information:\n\r{0}", engine::toString( _ev ) );

            for ( auto& _layer : _layers )
            {
                if ( !_layer->active() )
                    continue;

                if ( _layer->onEvent( _ev ) )
                    return;
            }
        }
        else if ( action == ENGINE_KEY_REPEAT ) 
        {
            auto _ev = CKeyPressedEvent( _name, key );
            //// ENGINE_CORE_TRACE( "event information:\n\r{0}", engine::toString( _ev ) );

            for ( auto& _layer : _layers )
            {
                if ( !_layer->active() )
                    continue;

                if ( _layer->onEvent( _ev ) )
                    return;
            }
        }

        /* call extra user-installed callbacks */
        for ( auto _callback : _app->m_keyboardCallbacks )
            _callback( key, action );

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
                if ( !_layer->active() )
                    continue;

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
                if ( !_layer->active() )
                    continue;

                _handled = _layer->onEvent( _ev );
                if ( _handled )
                    break;
            }
        };

        /* call extra user-installed callbacks */
        for ( auto _callback : _app->m_mouseButtonCallbacks )
            _callback( button, ( _handled ) ? ENGINE_MOUSE_BUTTON_RELEASED : action, x, y );

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
        {
            if ( !_layer->active() )
                continue;

            if ( _layer->onEvent( _ev ) )
                return;
        }

        /* call extra user-installed callbacks */
        for ( auto _callback : _app->m_mouseMoveCallbacks )
            _callback( x, y );

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
        {
            if ( !_layer->active() )
                continue;

            if ( _layer->onEvent( _ev ) )
                return;
        }

        /* call extra user-installed callbacks */
        for ( auto _callback : _app->m_scrollCallbacks )
            _callback( xOff, yOff );

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

        /* all layers should receive the resize-event */
        for ( auto& _layer : _layers )
            _layer->onEvent( _ev );

        /* all resources on the scene should be notified that the dimensions have changed */
        if ( _scene )
            _scene->resize( width, height );

        /* the renderer viewport should also change accordingly  */
        auto& _renderOptions = _app->m_renderOptions;
        _renderOptions.viewportWidth = width;
        _renderOptions.viewportHeight = height;

        /* call extra user-installed callbacks */
        for ( auto _callback : _app->m_resizeCallbacks )
            _callback( width, height );
    }

    std::vector< CImGuiLayer* > CApplication::guiLayers() const
    {
        std::vector< CImGuiLayer* > _layersPtrs;
        for ( auto& layer : m_guiLayers )
            _layersPtrs.push_back( layer.get() );

        return _layersPtrs;
    }

    CFrameBuffer* CApplication::_createRenderTarget()
    {
        engine::CAttachmentConfig _fbColorConfig;
        _fbColorConfig.name                 = "color_attachment";
        _fbColorConfig.attachment           = engine::eFboAttachment::COLOR;
        _fbColorConfig.width                = m_window->width();
        _fbColorConfig.height               = m_window->height();
        _fbColorConfig.texInternalFormat    = engine::eTextureFormat::RGB;
        _fbColorConfig.texFormat            = engine::eTextureFormat::RGB;
        _fbColorConfig.texPixelDataType     = engine::ePixelDataType::UINT_8;
        _fbColorConfig.texWrapU             = engine::eTextureWrap::REPEAT;
        _fbColorConfig.texWrapV             = engine::eTextureWrap::REPEAT;

        // @todo: sould use render-buffer-objects for depth attachment instead (quicker access)
        engine::CAttachmentConfig _fbDepthConfig;
        _fbDepthConfig.name                 = "depth_attachment";
        _fbDepthConfig.attachment           = engine::eFboAttachment::DEPTH;
        _fbDepthConfig.width                = m_window->width();
        _fbDepthConfig.height               = m_window->height();
        _fbDepthConfig.texInternalFormat    = engine::eTextureFormat::DEPTH;
        _fbDepthConfig.texFormat            = engine::eTextureFormat::DEPTH;
        _fbDepthConfig.texPixelDataType     = engine::ePixelDataType::UINT_32;
        _fbDepthConfig.texWrapU             = engine::eTextureWrap::REPEAT;
        _fbDepthConfig.texWrapV             = engine::eTextureWrap::REPEAT;

        auto _frameBuffer = new CFrameBuffer();
        _frameBuffer->addAttachment( _fbColorConfig );
        _frameBuffer->addAttachment( _fbDepthConfig );

        return _frameBuffer;
    }
}
