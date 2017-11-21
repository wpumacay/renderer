

#include <core/LBaseApp.h>

#include "miniengine/include/LScene.h"
#ifdef USE_MODERN_OPENGL
#include "miniengine/include/LShaderManager.h"
#endif
#include "miniengine/include/LMeshBuilder.h"

using namespace std;

#define NUM_LETTERS 4

class Hw1App : public engine::core::LBaseApp
{
    private :

    miniengine::LMesh* m_letters[NUM_LETTERS];

    miniengine::LScene* m_scene;
    miniengine::LMesh* m_testCube;

    Hw1App() : engine::core::LBaseApp()
    {
        #ifdef USE_MODERN_OPENGL
        miniengine::LShaderManager::create();
        #endif
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

        #ifdef USE_MODERN_OPENGL
        miniengine::LShaderManager::instance->initialize();
        #endif

        m_scene = new miniengine::LScene();

        miniengine::LBuildParams _params[NUM_LETTERS];
        //_params.b_width  = 1.0f;
        //_params.b_height = 1.0f;
        //_params.b_depth  = 1.0f;

        /// m_testCube = miniengine::LMeshBuilder::createMeshObject( miniengine::meshType::BOX,
        ///                                                          _params );

        _params[0].e_pPoints = miniengine::LMeshBuilder::s_letter_w_path;
        _params[0].e_nPoints = miniengine::LMeshBuilder::s_letter_w_num;
        _params[0].e_depth = 1.0f;

        _params[1].e_pPoints = miniengine::LMeshBuilder::s_letter_s_path;
        _params[1].e_nPoints = miniengine::LMeshBuilder::s_letter_s_num;
        _params[1].e_depth = 1.0f;

        _params[2].e_pPoints = miniengine::LMeshBuilder::s_letter_p_path;
        _params[2].e_nPoints = miniengine::LMeshBuilder::s_letter_p_num;
        _params[2].e_depth = 1.0f;

        _params[3].e_pPoints = miniengine::LMeshBuilder::s_letter_h_path;
        _params[3].e_nPoints = miniengine::LMeshBuilder::s_letter_h_num;
        _params[3].e_depth = 1.0f;

        GLfloat _posX[NUM_LETTERS] = { -3.0f, -1.0f, 1.0f, 3.0f };

        for ( int q = 0; q < NUM_LETTERS; q++ )
        {
            m_letters[q] = miniengine::LMeshBuilder::createMeshObject( miniengine::meshType::EXTRUSION,
                                                                       _params[q] );
            m_scene->addObject( m_letters[q] );
            m_letters[q]->pos.x = _posX[q];
        }


        //m_testCube = miniengine::LMeshBuilder::createMeshObject( miniengine::meshType::EXTRUSION,
        //                                                         _params );

        //m_scene->addObject( m_testCube );
    }

    void render() override
    {

        if ( m_scene != NULL )
        {
            m_scene->update( m_timeDelta );
        }

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // m_testCube->rot.x = m_timeNow;
        // m_testCube->rot.y = m_timeNow;
        // m_testCube->rot.z = m_timeNow;

        float _x = 2 * cos( m_timeNow );
        float _y = 2 * sin( m_timeNow );

        vector<miniengine::LLightSource*> _lights = m_scene->lights();

        for ( int q = 0; q < _lights.size(); q++ )
        {
            miniengine::LVec3 _pos = _lights[q]->getPosition();
            _pos.x = _x;
            //_pos.y = _y;
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

