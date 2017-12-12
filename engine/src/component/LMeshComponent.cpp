

#include "LMeshComponent.h"



namespace engine
{

        LMeshComponent::LMeshComponent( LEntity* pEntity, LMesh* pMesh )
            : LComponent( pEntity )
        {
            type = LMeshComponent::getStaticType();
            m_mesh = pMesh;
        }

        LMeshComponent::~LMeshComponent()
        {
            delete m_mesh;
        }


        void LMeshComponent::update( float dt )
        {
            m_mesh->pos = m_entity->pos;
            m_mesh->rotation = m_entity->rotation;
            m_mesh->scale = m_entity->scale;
        }
}