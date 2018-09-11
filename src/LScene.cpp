

#include "LScene.h"
#include "LFixedCamera3d.h"
#include "LMeshComponent.h"


namespace engine
{


    LScene::LScene()
    {
        m_currentCamera = NULL;
        m_fog = NULL;
        m_terrainGenerator = NULL;
        m_skybox = NULL;
    }

    LScene::~LScene()
    {
        for ( LICamera* _cam : m_cameras )
        {
            delete _cam;
        }

        for ( LILight* _light : m_lights )
        {
            delete _light;
        }

        for ( LEntity* _entity : m_entities )
        {
            delete _entity;
        }

        if ( m_terrainGenerator != NULL )
        {
            delete m_terrainGenerator;
            m_terrainGenerator = NULL;
        }

        if ( m_fog != NULL )
        {
            delete m_fog;
            m_fog = NULL;
        }

        if ( m_skybox != NULL )
        {
            delete m_skybox;
            m_skybox = NULL;
        }

        m_currentCamera = NULL;
    }

    void LScene::update( float dt )
    {
        for ( LEntity* _entity : m_entities )
        {
            _entity->update( dt );
        }

        for ( LICamera* _camera : m_cameras )
        {
            _camera->update( dt );
        }

        if ( m_terrainGenerator != NULL )
        {
            m_terrainGenerator->update( dt );
        }
    }

    void LScene::addFog( LFog* pFog )
    {
        if ( m_fog != NULL )
        {
            delete m_fog;
        }

        m_fog = pFog;
    }

}