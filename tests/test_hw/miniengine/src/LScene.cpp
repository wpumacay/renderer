
#include "../include/LScene.h"
#include <LConfig.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/ext.hpp>

using namespace std;

namespace miniengine
{


    LScene::LScene()
    {
        m_camera = new LCamera3D( LVec3( 0.0f, 0.0f, 0.0f ),
                                  LVec3( 0.0f, 1.0f, 0.0f ) );

        // Add a single default light
        LLightSource* _defLight = new LLightSource( LVec3( 0.0f, 1.0f, 0.0f ) );
        m_lights.push_back( _defLight );
    }

    LScene::~LScene()
    {
        for ( int q = 0; q < m_meshes.size(); q++ )
        {
            delete m_meshes[q];
            m_meshes[q] = NULL;
        }

        for ( int q = 0; q < m_lights.size(); q++ )
        {
            delete m_lights[q];
            m_lights[q] = NULL;
        }

        m_meshes.clear();
        m_lights.clear();

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

    void LScene::update( float dt )
    {

        if ( m_camera != NULL )
        {
            m_camera->update( dt );
        }
            
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

        // construct light props to pass to the render stage
        for ( int q = 0; q < m_lights.size(); q++ )
        {
            LLightProps _props;
            _props.ambient  = m_lights[q]->ambient;
            _props.diffuse  = m_lights[q]->diffuse;
            _props.specular = m_lights[q]->specular;
            _props.pos = m_lights[q]->getPosition();
            _props.dir = m_lights[q]->getDirection();

            _rInfo.lights.push_back( _props );
        }

        //cout << "projMat: " << endl;
        //cout << glm::to_string( _rInfo.projMat ) << endl;
        //cout << "viewMat: " << endl;
        //cout << glm::to_string( _rInfo.viewMat ) << endl;

        // render light lamps
        for ( int q = 0; q < m_lights.size(); q++ )
        {
            m_lights[q]->getLamp()->render( _rInfo );
        }

        // render object
        for ( int q = 0; q < m_meshes.size(); q++ )
        {
            m_meshes[q]->render( _rInfo );
        }

    }

}