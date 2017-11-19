
#include "../include/LScene.h"
#include <LConfig.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/ext.hpp>

using namespace std;

namespace miniengine
{


    LScene::LScene()
    {
        m_camera = new LCamera3D( LVec3( 1.0f, 1.0f, 5.0f ),
                                  LVec3( 0.0f, 1.0f, 0.0f ) );
    }

    LScene::~LScene()
    {
        for ( int q = 0; q < m_meshes.size(); q++ )
        {
            delete m_meshes[q];
            m_meshes[q] = NULL;
        }

        m_meshes.clear();

        if ( m_camera != NULL )
        {
            delete m_camera;
            m_camera = NULL;
        }
    }


    void LScene::addObject( LMesh* pObject )
    {
        m_meshes.push_back( pObject );
    }

    void LScene::addLight( LLightSource* pLight )
    {
        m_lights.push_back( pLight );
    }

    void LScene::render()
    {

        // Setup lights

        // Setup render info
        LRenderInfo _rInfo;
        _rInfo.projMat = glm::perspective( glm::radians( m_camera->fov ),
                                           ( float ) ENGINE_APP_WIDTH / ENGINE_APP_HEIGHT,
                                           0.1f, 100.0f );
        _rInfo.viewMat = m_camera->viewMatrix();

        //cout << "projMat: " << endl;
        //cout << glm::to_string( _rInfo.projMat ) << endl;
        //cout << "viewMat: " << endl;
        //cout << glm::to_string( _rInfo.viewMat ) << endl;

        // render object
        for ( int q = 0; q < m_meshes.size(); q++ )
        {
            m_meshes[q]->render( _rInfo );
        }

    }

}