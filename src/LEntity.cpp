

#include "LEntity.h"
#include "LMeshComponent.h"

using namespace std;

namespace engine
{

    LEntity::LEntity()
    {
        rotation = glm::mat4( 1.0f );
        scale = LVec3( 1.0f, 1.0f, 1.0f );
    }

    LEntity::LEntity( LMesh* pMesh )
    {
        rotation = glm::mat4( 1.0f );
        scale = LVec3( 1.0f, 1.0f, 1.0f );
        addComponent( new LMeshComponent( this, pMesh ) );
    }

    LEntity::~LEntity()
    {
        for ( auto _it = m_components.begin(); _it != m_components.end(); ++_it )
        {
            delete _it->second;
        }
    }

    void LEntity::addComponent( LComponent* pComponent )
    {
        m_components[pComponent->type] = pComponent;
    }

    void LEntity::update( float dt )
    {
        for ( auto _it = m_components.begin(); _it != m_components.end(); ++_it )
        {
            _it->second->update( dt );
        }
    }
}