

#include "LScene.h"
#include "LFixedCamera3d.h"
#include "LMeshComponent.h"


namespace engine
{


    LScene::LScene()
    {
        m_currentCamera = NULL;
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

        m_currentCamera = NULL;
    }

    void LScene::update( float dt )
    {
        for ( LEntity* _entity : m_entities )
        {
            _entity->update( dt );
        }
    }

    void LScene::enableLighting()
    {
        for ( LEntity* _entity : m_entities )
        {
            auto _meshComponent = _entity->getComponent<LMeshComponent>();
            if ( _meshComponent != NULL )
            {
                _meshComponent->getMesh()->enableLighting();
            }
        }
    }

    void LScene::disableLighting()
    {
        for ( LEntity* _entity : m_entities )
        {
            auto _meshComponent = _entity->getComponent<LMeshComponent>();
            if ( _meshComponent != NULL )
            {
                _meshComponent->getMesh()->disableLighting();
            }
        }
    }

    void LScene::setShadingMode( bool useSmoothShading )
    {
        for ( LEntity* _entity : m_entities )
        {
            auto _meshComponent = _entity->getComponent<LMeshComponent>();
            if ( _meshComponent != NULL )
            {
                _meshComponent->getMesh()->recomputeNormals( useSmoothShading );
            }
        }
    }

}