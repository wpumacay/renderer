
#include <core/COpenGLApp.h>

namespace engine
{
    COpenGLApp* COpenGLApp::s_instance = NULL;

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
        // initialize logger first, as other resources use it
        engine::CLogger::Init();

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

        m_windowPtr         = NULL;
        m_masterRenderer    = NULL;
        m_scenePtr          = NULL;
        m_uiPtr             = NULL;

        engine::DebugSystem::release();
        engine::InputSystem::release();
        engine::LShaderManager::release();
    }

    void COpenGLApp::init()
    {
        CWindowProps _windowProperties;
        _windowProperties.width = 1024;
        _windowProperties.height = 768;
        _windowProperties.title = "OpenGL Application window";
        _windowProperties.callbackKey = nullptr;
        _windowProperties.callbackMouse = nullptr;
        _windowProperties.callbackMouseMove = nullptr;

        m_windowPtr         = new COpenGLWindow( _windowProperties );
        m_masterRenderer    = new LMasterRenderer();
        m_scenePtr          = new LScene();
        m_uiPtr             = NULL; // let the user create its own specific UI

        engine::LShaderManager::create();
        engine::LAssetsManager::create();
        engine::InputSystem::init();
        engine::DebugSystem::init();

        m_windowPtr->registerKeyCallback( engine::LInputHandler::callback_key );
        m_windowPtr->registerMouseCallback( engine::LInputHandler::callback_mouse );
        m_windowPtr->registerMouseMoveCallback( engine::LInputHandler::callback_mouseMove );

        // let the user initialize its own stuff
        _initUser();

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
        // handle the usage of the cursor according to the current camera active mode
        if ( _currentCamera->type() == LFpsCamera::GetStaticType() )
        {
            if ( _currentCamera->isActive() )
                m_windowPtr->disableCursor();
            else
                m_windowPtr->enableCursor();
        }

        m_scenePtr->update( m_timeDelta );
        m_masterRenderer->render( m_scenePtr );

        if ( m_uiPtr )
            m_uiPtr->render();

        engine::DebugSystem::setupMatrices( _currentCamera->getViewMatrix(),
                                            _currentCamera->getProjectionMatrix() );
        engine::DebugSystem::render();
    }

    void COpenGLApp::end()
    {
        ENGINE_CORE_ASSERT( m_windowPtr, "Should have created a window by now" );

        m_windowPtr->end();

        m_timeDelta = min( (float)( glfwGetTime() - m_timeNow ), MAX_DELTA );
        m_timeNow = glfwGetTime();
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
