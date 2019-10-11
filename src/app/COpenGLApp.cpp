
#include <app/COpenGLApp.h>

namespace engine
{
    COpenGLApp* COpenGLApp::s_instance = nullptr;

    COpenGLApp* COpenGLApp::GetInstance()
    {
        ENGINE_CORE_ASSERT( COpenGLApp::s_instance, "Must have a valid application created" );

        return COpenGLApp::s_instance;
    }

    COpenGLWindow* COpenGLApp::GetWindow()
    {
        ENGINE_CORE_ASSERT( COpenGLApp::s_instance, "Must have a valid application created" );
        ENGINE_CORE_ASSERT( COpenGLApp::s_instance->m_windowPtr, "Must have a valid window created" );

        return COpenGLApp::s_instance->m_windowPtr.get();
    }

    COpenGLApp::COpenGLApp()
    {
        // initialize there resources first, as other resources use it
        engine::CLogger::Init();
        engine::CTime::Init();

        // create the global reference for other systems to use
        COpenGLApp::s_instance = this;

        m_timeNow = 0.0f;
        m_timeDelta = 0.0f;

        ENGINE_CORE_INFO( "GL-Application created" );
    }

    COpenGLApp::~COpenGLApp()
    {

        m_windowPtr         = nullptr;
        m_scenePtr          = nullptr;
        m_uiPtr             = nullptr;
        m_mainRendererPtr   = nullptr;

        engine::CTime::Release();
        engine::CDebugDrawer::Release();
        engine::CInputHandler::Release();
        engine::CShaderManager::Release();
        engine::CTextureManager::Release();
        engine::CNoiseGenerator::Release();
    }

    void COpenGLApp::init()
    {
        CWindowProps _windowProperties;
        _windowProperties.width = 800;
        _windowProperties.height = 600;
        _windowProperties.title = "OpenGL Application window";
        _windowProperties.callbackKey = nullptr;
        _windowProperties.callbackMouse = nullptr;
        _windowProperties.callbackMouseMove = nullptr;

        m_windowPtr         = std::unique_ptr< COpenGLWindow >( new COpenGLWindow( _windowProperties ) );
        m_scenePtr          = std::unique_ptr< CScene >( new CScene() );
        m_uiPtr             = nullptr; // let the user create its own specific UI
        m_mainRendererPtr   = std::unique_ptr< CMainRenderer >( new CMainRenderer() );

        engine::CTextureManager::Init();
        engine::CShaderManager::Init();
        engine::CInputHandler::Init();
        engine::CDebugDrawer::Init();
        engine::CNoiseGenerator::Init();

        m_windowPtr->registerKeyCallback( engine::CInputHandler::callback_key );
        m_windowPtr->registerMouseCallback( engine::CInputHandler::callback_mouse );
        m_windowPtr->registerMouseMoveCallback( engine::CInputHandler::callback_mouseMove );
        m_windowPtr->registerScrollCallback( engine::CInputHandler::callback_scroll );
        m_windowPtr->registerResizeCallback( []( int width, int height )
            {
                auto _app = COpenGLApp::GetInstance();
                auto _scene = _app->scene();
                _scene->resize( width, height );
            } );

        // let the user initialize its own stuff
        _initUser();

        // start keeping track of time
        engine::CTime::Start();

        ENGINE_CORE_INFO( "GL-Application started successfully" );
    }

    void COpenGLApp::update()
    {
        ENGINE_CORE_ASSERT( m_scenePtr, "Need a scene object to update" );

        // let the user update its own logic
        _updateUser();

        // update internal properties of the scene (cameras, lights, etc.)
        m_scenePtr->update();

        // handle the usage of the cursor according to the current camera active mode
        auto _currentCamera = m_scenePtr->currentCamera();
        if ( !_currentCamera )
            return;

        if ( _currentCamera->type() == CFpsCamera::GetStaticType() )
        {
            if ( _currentCamera->active() )
                m_windowPtr->disableCursor();
            else
                m_windowPtr->enableCursor();
        }
    }

    void COpenGLApp::beginRendering()
    {
        ENGINE_CORE_ASSERT( m_windowPtr, "Should have created a window by now" );

        m_windowPtr->begin();

        m_timeNow = glfwGetTime();
    }

    void COpenGLApp::renderScene()
    {
        ENGINE_CORE_ASSERT( m_scenePtr, "Need a scene object to render" );

        // auto _currentCamera = m_scenePtr->currentCamera();
        // if ( _currentCamera )
        //     m_mainRendererPtr->render( m_scenePtr, _currentCamera );

    }

    void COpenGLApp::renderDebug()
    {
        auto _currentCamera = m_scenePtr->currentCamera();
        if ( _currentCamera )
            engine::CDebugDrawer::Render( _currentCamera );
    }

    void COpenGLApp::renderUi()
    {
        if ( !m_uiPtr )
            return;

//         auto _currentCamera = m_scenePtr->getCurrentCamera();
//         bool _hasCamera = ( _currentCamera != nullptr );
//         bool _hasActiveCamera = ( _hasCamera ) ? _currentCamera->active() : false;
//         bool _hasUserControlledCamera = ( _hasCamera ) ? ( _currentCamera->type() == CFpsCamera::GetStaticType() ||
//                                                            _currentCamera->type() == COrbitCamera::GetStaticType() ) : false;
// 
//         if ( _hasCamera && _hasActiveCamera && _hasUserControlledCamera )
//             return;

        m_uiPtr->render();
    }

    void COpenGLApp::endRendering()
    {
        ENGINE_CORE_ASSERT( m_windowPtr, "Should have created a window by now" );

        m_windowPtr->end();

        m_timeDelta = glfwGetTime() - m_timeNow;
        m_timeNow = glfwGetTime();

        engine::CTime::Update( m_timeDelta );
    }

    void COpenGLApp::_initUser()
    {
        // Override this method with extra initialization logic after gl-resources have been creted
    }

    void COpenGLApp::_updateUser()
    {
        // Override this method with extra update logic prior to rendering
    }

}
