
#include <app/CApplication.h>

namespace engine
{
    CApplication* CApplication::s_instance = nullptr;

    CApplication* CApplication::GetInstance()
    {
        ENGINE_CORE_ASSERT( CApplication::s_instance, "Must have a valid application-instance created" );

        return CApplication::s_instance;
    }

#ifndef ENGINE_HEADLESS_EGL
    CApplication::CApplication( const CWindowProps& windowProperties )
        : CApplication( windowProperties, CImGuiProps() ) {}

    CApplication::CApplication( const CWindowProps& windowProperties,
                                const CImGuiProps& imguiProperties )
#else
    CApplication::CApplication( const CWindowProps& windowProperties )
#endif /* ENGINE_HEADLESS_EGL */
    {
        // create the global reference for other systems to use
        CApplication::s_instance = this;

        tinyutils::Logger::Init();
        ENGINE_CORE_INFO( "GL-Application: starting..." );
    #ifdef ENGINE_HEADLESS_EGL
        m_window = std::make_unique< CWindowEGL >( windowProperties );
    #else
        m_window = std::make_unique< CWindowGLFW >( windowProperties );
        ENGINE_CORE_ASSERT( dynamic_cast<CWindowGLFW*>( m_window.get() )->glfwWindow(), "There was an error while creating the opengl-window" );
    #endif /* ENGINE_HEADLESS_EGL */

        tinyutils::Profiler::Init();
        tinyutils::Profiler::BeginSession( "sess_core_init" );
        ENGINE_PROFILE_FUNCTION_IN_SESSION( "sess_core_init" );
        ENGINE_CORE_INFO( "GL-Application: profiling enabled" );

        engine::CTextureManager::Init();
        engine::CShaderManager::Init();
        engine::CInputManager::Init();
        engine::CDebugDrawer::Init();
        tinyutils::PerlinNoise::Init();
        tinyutils::Clock::Init();

        m_scene = std::make_unique< CScene >();
        m_mainRenderer = std::make_unique< CMainRenderer >();
    #ifndef ENGINE_HEADLESS_EGL
        m_imguiManager  = std::make_unique< CImGuiManager >( dynamic_cast<CWindowGLFW*>( m_window.get() )->glfwWindow(), imguiProperties );
    #endif /* ENGINE_HEADLESS_EGL */
        m_renderTarget  = _createRenderTarget();
        m_useRenderTarget = false;

        m_window->registerKeyCallback( &CApplication::CallbackKey );
        m_window->registerMouseCallback( &CApplication::CallbackMouseButton );
        m_window->registerMouseMoveCallback( &CApplication::CallbackMouseMove );
        m_window->registerScrollCallback( &CApplication::CallbackScroll );
        m_window->registerResizeCallback( &CApplication::CallbackResize );

        // setup initial viewport
        m_renderOptions.viewportWidth = m_window->width();
        m_renderOptions.viewportHeight = m_window->height();
        m_renderOptions.renderTargetPtr = nullptr;

        m_timeStamp = 0.0;
    #ifndef ENGINE_HEADLESS_EGL
        // create a utils panel by default (the user can choose to use it or not)
        m_guiUtilsLayer = new CImGuiUtilsLayer( "Utils-layer",
                                                m_scene.get(),
                                                m_mainRenderer.get(),
                                                &m_renderOptions,
                                                m_imguiManager.get(),
                                                m_window.get() );

        m_guiUtilsLayer->setActive( false );
        addGuiLayer( std::unique_ptr< CImGuiLayer >( m_guiUtilsLayer ) );
    #endif /* ENGINE_HEADLESS_EGL */
        ENGINE_CORE_INFO( "GL-Application started successfully!!!" );
        // flush results from initialization
        tinyutils::Profiler::EndSession( "sess_core_init" );
    }

    CApplication::CApplication()
        : CApplication( CWindowProps() ) {}

    CApplication::~CApplication()
    {
        ENGINE_CORE_INFO( "GL-Application: destroying..." );

        m_scene         = nullptr;
        m_mainRenderer  = nullptr;
    #ifndef ENGINE_HEADLESS_EGL
        m_imguiManager  = nullptr;
    #endif /* ENGINE_HEADLESS_EGL */

        // Release any gl-resources before destroying the context
        m_renderTarget = nullptr;
        engine::CDebugDrawer::Release();
        engine::CInputManager::Release();
        engine::CShaderManager::Release();
        engine::CTextureManager::Release();

        // Destroy the window (and the context as well)
        m_window = nullptr;

        tinyutils::Clock::Release();
        tinyutils::Profiler::Release();
        tinyutils::PerlinNoise::Release();
        tinyutils::Logger::Release();
    }

    CScene* CApplication::setScene( std::unique_ptr< CScene > scene )
    {
        m_scene = std::move( scene );
    #ifndef ENGINE_HEADLESS_EGL
        // update resources that require the scene reference
        m_guiUtilsLayer->setScene( m_scene.get() );
    #endif /* ENGINE_HEADLESS_EGL */
        // return a reference for the user to play with
        return m_scene.get();
    }

    void CApplication::setOffscreenRendering( bool enabled )
    {
        m_useRenderTarget = enabled;
        m_renderOptions.renderTargetPtr = ( enabled ) ? m_renderTarget.get() : nullptr;
    }
#ifndef ENGINE_HEADLESS_EGL
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

    CImGuiLayer* CApplication::addGuiLayer( std::unique_ptr< CImGuiLayer > layer )
    {
        m_guiLayers.push_back( std::move( layer ) );
        // return a reference for the user to play with
        return m_guiLayers.back().get();
    }
#endif /* ENGINE_HEADLESS_EGL */

    void CApplication::update()
    {
        // update scene resources (cameras, ...)
        if ( m_scene )
            m_scene->Update();

    #ifndef ENGINE_HEADLESS_EGL
        // update all gui-layers
        if ( m_imguiManager->active() )
            for ( auto& layer : m_guiLayers )
                if ( layer->active() )
                    layer->update();
    #endif
    }

    void CApplication::begin()
    {
        tinyutils::Profiler::BeginSession( "sess_core_render" );
        auto tpStart = std::chrono::high_resolution_clock::now();
        m_timeStamp = std::chrono::time_point_cast<std::chrono::microseconds>( tpStart ).time_since_epoch().count() * 0.001;
        // prepare window for rendering, and poll events
        m_window->begin();

        // configure some resources (if available) required for rendering
        if ( m_scene && m_scene->HasCurrentCamera() )
            m_renderOptions.cameraPtr = m_scene->GetCurrentCamera();

        if ( m_scene && m_scene->HasCurrentLight() )
        {
            auto _light = m_scene->GetCurrentLight();
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

        if ( m_scene && m_scene->HasCurrentSkybox() )
            m_renderOptions.skyboxPtr = m_scene->GetCurrentSkybox();

        if ( m_scene && m_scene->HasCurrentFog() )
            m_renderOptions.fogPtr = m_scene->GetCurrentFog();

        if ( m_scene && m_mainRenderer && m_mainRenderer->shadowMap() )
            m_renderOptions.shadowMapPtr = m_mainRenderer->shadowMap();

        // prepare our renderer to accepts rendering commands
        m_mainRenderer->begin( m_renderOptions );
    }

    void CApplication::render()
    {
        ENGINE_CORE_ASSERT( m_scene, "There must be a valid scene for this application" );
        ENGINE_PROFILE_FUNCTION_IN_SESSION( "sess_core_render" );

        // submit the renderables of this scene to the renderer
        m_mainRenderer->submit( m_scene->GetRenderablesList() );

        // render all commands requested to our renderer
        m_mainRenderer->render();
    #ifndef ENGINE_HEADLESS_EGL
        // prepare imgui prior to accepts any rendering commands
        m_imguiManager->begin();
        // render all gui-layers
        if ( m_imguiManager->active() )
            for ( auto& layer : m_guiLayers )
                if ( layer->active() )
                    layer->render();
    #endif /* ENGINE_HEADLESS_EGL */

        int32 _prevViewportX, _prevViewportY, _prevViewportWidth, _prevViewportHeight;
        if ( m_useRenderTarget && m_renderTarget )
        {
            int32 _currentViewport[4];
            glGetIntegerv( GL_VIEWPORT, _currentViewport );
            _prevViewportX = _currentViewport[0];
            _prevViewportY = _currentViewport[1];
            _prevViewportWidth  = _currentViewport[2];
            _prevViewportHeight = _currentViewport[3];

            glViewport( 0, 0, m_renderOptions.viewportWidth, m_renderOptions.viewportHeight );
            m_renderTarget->bind();
        }

        // render all commands requested to the debug drawer
        if ( m_scene->HasCurrentCamera() && m_scene->HasCurrentLight() )
            CDebugDrawer::Render( m_scene->GetCurrentCamera(), m_scene->GetCurrentLight() );
        else if ( m_scene->HasCurrentCamera() )
            CDebugDrawer::Render( m_scene->GetCurrentCamera() );

        if ( m_useRenderTarget && m_renderTarget )
        {
            glViewport( _prevViewportX, _prevViewportY, _prevViewportWidth, _prevViewportHeight );
            m_renderTarget->unbind();
        }

    #ifndef ENGINE_HEADLESS_EGL
        // render all commands requested to imgui so far
        m_imguiManager->render();
    #endif /* ENGINE_HEADLESS_EGL */
    }

    void CApplication::end()
    {
        // swap buffers such that we see that juicy frame (if we rendered to default target)
        m_window->end();

        auto tpEnd = std::chrono::high_resolution_clock::now();
        float64 _timeNow = std::chrono::time_point_cast<std::chrono::microseconds>( tpEnd ).time_since_epoch().count() * 0.001;
        float64 _timeDelta = _timeNow - m_timeStamp;
        m_timeStamp = _timeNow;

        tinyutils::Profiler::EndSession( "sess_core_render" );
    }

    void CApplication::Init()
    {
        _InitUser();
    }

    void CApplication::Run()
    {
        while( active() )
        {
            // Start tracking time (beginning of render-loop)
            tinyutils::Clock::Tick();
            // Let user handle its own render-loop
            _RunUser();
            // Stop tracking time (end of render-loop)
            tinyutils::Clock::Tock();
        }
    }

    void CApplication::CallbackKey( int key, int action )
    {
        auto _app = CApplication::GetInstance();
    #ifndef ENGINE_HEADLESS_EGL
        auto& _layers = _app->m_guiLayers;
        auto _name = std::string( "key_event: " ) + std::to_string( key ) + " - " + std::to_string( action );

        if ( action == KeyAction::KEY_PRESSED ) 
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
        else if ( action == KeyAction::KEY_RELEASED ) 
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
        else if ( action == KeyAction::KEY_REPEAT ) 
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
    #endif /* ENGINE_HEADLESS_EGL */

        /* call extra user-installed callbacks */
        for ( auto _callback : _app->m_keyboardCallbacks )
            _callback( key, action );

        CInputManager::Callback_key( key, action );
    }

    void CApplication::CallbackMouseButton( int button, int action, double x, double y )
    {
        auto _app = CApplication::GetInstance();
        bool _handled = false;
    #ifndef ENGINE_HEADLESS_EGL
        auto& _layers = _app->m_guiLayers;
        auto _name = std::string( "mouse_button_event: " ) + std::to_string( button ) + " - " + 
                            std::to_string( action ) + " - " + std::to_string( x ) + " - " + std::to_string( y );

        if ( action == MouseAction::BUTTON_PRESSED ) 
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
        else if ( action == MouseAction::BUTTON_RELEASED ) 
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
        }
    #endif /* ENGINE_HEADLESS_EGL */

        /* call extra user-installed callbacks */
        for ( auto _callback : _app->m_mouseButtonCallbacks )
            _callback( button, ( _handled ) ? MouseAction::BUTTON_RELEASED : action, x, y );

        if ( _handled ) // clear the state for this button
            CInputManager::Callback_mouse( button, MouseAction::BUTTON_RELEASED, x, y );
        else
            CInputManager::Callback_mouse( button, action, x, y );
    }

    void CApplication::CallbackMouseMove( double x, double y )
    {
        auto _app = CApplication::GetInstance();
    #ifndef ENGINE_HEADLESS_EGL
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
    #endif /* ENGINE_HEADLESS_EGL */

        /* call extra user-installed callbacks */
        for ( auto _callback : _app->m_mouseMoveCallbacks )
            _callback( x, y );

        CInputManager::Callback_mouseMove( x, y );
    }

    void CApplication::CallbackScroll( double xOff, double yOff )
    {
        auto _app = CApplication::GetInstance();
    #ifndef ENGINE_HEADLESS_EGL
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
    #endif /* ENGINE_HEADLESS_EGL */

        /* call extra user-installed callbacks */
        for ( auto _callback : _app->m_scrollCallbacks )
            _callback( xOff, yOff );

        CInputManager::Callback_scroll( xOff, yOff );
    }

    void CApplication::CallbackResize( int width, int height )
    {
        auto _app = CApplication::GetInstance();
        auto _scene = _app->m_scene.get();

    #ifndef ENGINE_HEADLESS_EGL
        auto& _layers = _app->m_guiLayers;
        auto _name = std::string( "resize_event: " ) + std::to_string( width ) + " - " + std::to_string( height );

        auto _ev = CResizeEvent( _name, width, height );
        //// ENGINE_CORE_TRACE( "event information:\n\r{0}", engine::toString( _ev ) );

        /* all layers should receive the resize-event */
        for ( auto& _layer : _layers )
            _layer->onEvent( _ev );
    #endif /* ENGINE_HEADLESS_EGL */

        /* all resources on the scene should be notified that the dimensions have changed */
        if ( _scene )
            _scene->Resize( width, height );

        /* the renderer viewport should also change accordingly  */
        auto& _renderOptions = _app->m_renderOptions;
        _renderOptions.viewportWidth = width;
        _renderOptions.viewportHeight = height;

        /* the offscreen render target should be resized as well */
        if ( _app->m_renderTarget )
            _app->m_renderTarget->resize( width, height );

        /* call extra user-installed callbacks */
        for ( auto _callback : _app->m_resizeCallbacks )
            _callback( width, height );
    }

#ifndef ENGINE_HEADLESS_EGL
    std::vector< CImGuiLayer* > CApplication::guiLayers() const
    {
        std::vector< CImGuiLayer* > _layersPtrs;
        for ( auto& layer : m_guiLayers )
            _layersPtrs.push_back( layer.get() );

        return _layersPtrs;
    }
#endif

    std::unique_ptr<CFrameBuffer> CApplication::_createRenderTarget()
    {
        engine::CAttachmentConfig _fbColorConfig;
        _fbColorConfig.name                 = "color_attachment";
        _fbColorConfig.attachment           = engine::eFboAttachment::COLOR;
        _fbColorConfig.texInternalFormat    = engine::eTextureFormat::RGB;
        _fbColorConfig.texFormat            = engine::eTextureFormat::RGB;
        _fbColorConfig.texPixelDataType     = engine::ePixelDataType::UINT_8;
        _fbColorConfig.texWrapU             = engine::eTextureWrap::REPEAT;
        _fbColorConfig.texWrapV             = engine::eTextureWrap::REPEAT;

        // @todo: sould use render-buffer-objects for depth attachment instead (quicker access)
        engine::CAttachmentConfig _fbDepthConfig;
        _fbDepthConfig.name                 = "depth_attachment";
        _fbDepthConfig.attachment           = engine::eFboAttachment::DEPTH;
        _fbDepthConfig.texInternalFormat    = engine::eTextureFormat::DEPTH;
        _fbDepthConfig.texFormat            = engine::eTextureFormat::DEPTH;
        _fbDepthConfig.texPixelDataType     = engine::ePixelDataType::UINT_32;
        _fbDepthConfig.texWrapU             = engine::eTextureWrap::REPEAT;
        _fbDepthConfig.texWrapV             = engine::eTextureWrap::REPEAT;

        auto _frameBuffer = std::make_unique<CFrameBuffer>( m_window->width(), m_window->height() );
        _frameBuffer->addAttachment( _fbColorConfig );
        _frameBuffer->addAttachment( _fbDepthConfig );

        return std::move( _frameBuffer );
    }
}
