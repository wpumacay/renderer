

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

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        m_testCube->rot.x = m_timeNow;
        m_testCube->rot.y = m_timeNow;
        m_testCube->rot.z = m_timeNow;

        m_scene->render();
    }

};




int main()
{

    Hw1App::create();
    Hw1App::instance->loop();
    Hw1App::destroy();

    return 0;
}

