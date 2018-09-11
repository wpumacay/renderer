
#pragma once

#include "LCommon.h"
#include "LComponent.h"
#include "LMesh.h"
#include <unordered_map>

using namespace std;

namespace engine
{

    class LComponent;

    class LEntity
    {

        protected :

        unordered_map< string, LComponent* > m_components;

        public :

        LVec3 pos;
        glm::mat4 rotation;
        LVec3 scale;

        LEntity();
        LEntity( LMesh* pMesh );
        ~LEntity();

        template< class T >
        T* getComponent()
        {
            string _type = T::getStaticType();
            auto _it = m_components.find( _type );
            if ( _it == m_components.end() )
            {
                return NULL;
            }

            return ( T* ) _it->second;
        }

        void addComponent( LComponent* pComponent );

        virtual void update( float dt );
    };




}