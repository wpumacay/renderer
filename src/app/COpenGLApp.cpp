
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

        return COpenGLApp::s_instance->m_windowPtr;
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
        if ( m_windowPtr )
            delete m_windowPtr;

        if ( m_masterRenderer )
            delete m_masterRenderer;

        if ( m_scenePtr )
            delete m_scenePtr;

        if ( m_uiPtr )
            delete m_uiPtr;

        m_windowPtr         = nullptr;
        m_masterRenderer    = nullptr;
        m_scenePtr          = nullptr;
        m_uiPtr             = nullptr;

        engine::CTime::Release();
        engine::CDebugDrawer::Release();
        engine::CInputHandler::Release();
        engine::LShaderManager::release();
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

        m_windowPtr         = new COpenGLWindow( _windowProperties );
        m_masterRenderer    = new LMasterRenderer();
        m_scenePtr          = new LScene();
        m_uiPtr             = nullptr; // let the user create its own specific UI

        engine::CTextureManager::Init();
        engine::LShaderManager::create();
        engine::CInputHandler::Init();
        engine::CDebugDrawer::Init();
        engine::CNoiseGenerator::Init();

        m_windowPtr->registerKeyCallback( engine::CInputHandler::callback_key );
        m_windowPtr->registerMouseCallback( engine::CInputHandler::callback_mouse );
        m_windowPtr->registerMouseMoveCallback( engine::CInputHandler::callback_mouseMove );
        m_windowPtr->registerScrollCallback( engine::CInputHandler::callback_scroll );

        // let the user initialize its own stuff
        _initUser();

        // start keeping track of time
        engine::CTime::Start();

        ENGINE_CORE_INFO( "GL-Application started successfully" );
    }

    void COpenGLApp::begin()
    {
        ENGINE_CORE_ASSERT( m_windowPtr, "Should have created a window by now" );

        m_windowPtr->begin();

        m_timeNow = glfwGetTime();
    }

    void COpenGLApp::update()
    {
        ENGINE_CORE_ASSERT( m_scenePtr, "Need a scene object to render" );

        // let the user update its own logic
        _updateUser();

        auto _currentCamera = m_scenePtr->getCurrentCamera();
        if ( _currentCamera )
        {
            // handle the usage of the cursor according to the current camera active mode
            if ( _currentCamera->type() == CFpsCamera::GetStaticType() )
            {
                if ( _currentCamera->active() )
                    m_windowPtr->disableCursor();
                else
                    m_windowPtr->enableCursor();
            }

            m_scenePtr->update( m_timeDelta );
            m_masterRenderer->render( m_scenePtr );

            engine::CDebugDrawer::Render( _currentCamera );
        }

        if ( m_uiPtr )
            m_uiPtr->render();
    }

    void COpenGLApp::end()
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

    LScene* COpenGLApp::scene()
    {
        return m_scenePtr;
    }

    bool COpenGLApp::isActive()
    {
        return m_windowPtr->isActive();
    }

    float COpenGLApp::frametime()
    {
        return m_timeDelta;
    }

    float COpenGLApp::fps()
    {
        return 1.0f / m_timeDelta;
    }
}
