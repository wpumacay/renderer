

#include <core/LBaseApp.h>

#include "miniengine/include/LScene.h"
#include "miniengine/include/LShaderManager.h"
#include "miniengine/include/LMeshBuilder.h"

using namespace std;

class Hw1App : public engine::core::LBaseApp
{
    private :

    miniengine::LScene* m_scene;
    miniengine::LMesh* m_testCube;

    Hw1App() : engine::core::LBaseApp()
    {
        miniengine::LShaderManager::create();
    }

    public :

    static void create()
    {
        if ( Hw1App::instance != NULL )
        {
            delete Hw1App::instance;
        }

        Hw1App::instance = new Hw1App();
        Hw1App::instance->init();
    }

    void onStartup() override
    {
        ENGINE_LOG( "Hw1App::onStartup> initializing some custom stuff" );

        glEnable( GL_DEPTH_TEST );
        glDisable( GL_CULL_FACE );

        miniengine::LShaderManager::instance->initialize();

        m_scene = new miniengine::LScene();

        miniengine::LBuildParams _params;
        _params.b_width  = 1.0f;
        _params.b_height = 1.0f;
        _params.b_depth  = 1.0f;

        m_testCube = miniengine::LMeshBuilder::createMeshObject( miniengine::meshType::BOX,
                                                                 _params );

        m_scene->addObject( m_testCube );
    }

    void render() override
    {

        if ( m_scene != NULL )
        {
            m_scene->update( m_timeDelta );
        }

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        m_testCube->rot.x = m_timeNow;
        m_testCube->rot.y = m_timeNow;
        m_testCube->rot.z = m_timeNow;

        float _x = 2 * cos( m_timeNow );
        float _y = 2 * sin( m_timeNow );

        vector<miniengine::LLightSource*> _lights = m_scene->lights();

        for ( int q = 0; q < _lights.size(); q++ )
        {
            miniengine::LVec3 _pos = _lights[q]->getPosition();
            //_pos.x = _x;
            _lights[q]->setPosition( _pos );
        }

        m_scene->render();
    }

    void onKeyCallback( int pKey, int pScancode, 
                        int pAction, int pMode ) override
    {
        if ( m_scene == NULL )
        {
            return;
        }

        if ( pAction == GLFW_PRESS )
        {
            m_scene->onKeyDown( pKey );
        }
        else if ( pAction == GLFW_RELEASE )
        {
            m_scene->onKeyUp( pKey );
        }
    }

    void onMouseButtonCallback( int pButton, 
                                int pAction, 
                                int pMods ) override
    {
        if ( m_scene == NULL )
        {
            return;
        }

        double evx, evy;

        glfwGetCursorPos( m_window, &evx, &evy );

        if ( pAction == GLFW_PRESS )
        {
            m_scene->onMouseDown( (float)evx, (float)evy );
        }
        else if ( pAction == GLFW_RELEASE )
        {
            m_scene->onMouseUp( (float)evx, (float)evy );
        }
    }

    void onCursorCallback( double x, double y ) override
    {
        if ( m_scene == NULL )
        {
            return;
        }

        m_scene->onMouseMove( x, y );
    }

    void onMouseScrollCallback( double xOff, double yOff ) override
    {
        if ( m_scene == NULL )
        {
            return;
        }

        m_scene->onMouseScroll( xOff, yOff );
    }

};




int main()
{

    Hw1App::create();
    Hw1App::instance->loop();
    Hw1App::destroy();

    return 0;
}

