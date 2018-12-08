
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
        m_window = LWindow::GetInstance();
        m_masterRenderer = LMasterRenderer::GetInstance();
        m_scene = new LScene();

        engine::LShaderManager::create();
        engine::LAssetsManager::create();
        engine::InputSystem::init();
        engine::DebugSystem::init();

        m_tBef = 0.0f;
        m_tNow = 0.0f;
        m_tDelta = 0.0f;
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
    }

    void LApp::begin()
    {
        m_window->clear();
        m_window->pollEvents();

        m_tNow = glfwGetTime();
        m_tDelta = min( m_tNow - m_tBef, MAX_DELTA );
        m_tBef = m_tNow;

        // std::cout << "dt: " << m_tDelta << std::endl;
    }

    void LApp::update()
    {
        if ( m_scene != NULL )
        {
            m_scene->update( m_tDelta );
            m_masterRenderer->render( m_scene );

            auto _camera = m_scene->getCurrentCamera();
            engine::DebugSystem::setupMatrices( _camera->getViewMatrix(),
                                                _camera->getProjectionMatrix() );
            engine::DebugSystem::render();
        }
    }

    void LApp::end()
    {
        m_window->swapBuffers();
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
