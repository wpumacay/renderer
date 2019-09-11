
#include <LApp.h>


namespace engine
{

    LApp* LApp::_INSTANCE = NULL;

    LApp* LApp::GetInstance()
    {
        if ( LApp::_INSTANCE == NULL )
        {
            LApp::_INSTANCE = new LApp();
        }

        return LApp::_INSTANCE;
    }

    LApp::LApp()
    {
        // initialize logger first, as other resources use it
        engine::CLogger::Init();

        m_window = new LWindow( { 1024, 768, "Application window" } );
        m_masterRenderer = LMasterRenderer::GetInstance();
        m_scene = new LScene();

        engine::LShaderManager::create();
        engine::LAssetsManager::create();
        engine::InputSystem::init();
        engine::DebugSystem::init();

        m_tBef = 0.0f;
        m_tNow = 0.0f;
        m_tDelta = 0.0f;

        ENGINE_CORE_INFO( "engine application started successfully" );
    }

    LApp::~LApp()
    {
        if ( m_scene != NULL )
        {
            delete m_scene;
            m_scene = NULL;
        }

        if ( m_window != NULL )
        {
            delete m_window;
            m_window = NULL;
        }

        engine::DebugSystem::release();
        engine::InputSystem::release();
        engine::LShaderManager::release();

        LApp::_INSTANCE = NULL;
    }

    void LApp::begin()
    {
        m_window->begin();

        m_tNow = glfwGetTime();
        m_tDelta = min( m_tNow - m_tBef, MAX_DELTA );
        m_tBef = m_tNow;
    }

    void LApp::update()
    {
        assert( m_scene != NULL );

        m_scene->update( m_tDelta );
        m_masterRenderer->render( m_scene );

        auto _camera = m_scene->getCurrentCamera();
        engine::DebugSystem::setupMatrices( _camera->getViewMatrix(),
                                            _camera->getProjectionMatrix() );
        engine::DebugSystem::render();
    }

    void LApp::end()
    {
        m_window->end();
    }

    LScene* LApp::scene()
    {
        return m_scene;
    }

    bool LApp::isActive()
    {
        return m_window->isActive();
    }

    LWindow* LApp::window()
    {
        return m_window;
    }
}
