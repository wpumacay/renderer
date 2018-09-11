
#pragma once

#include "LCommon.h"
#include "LEntity.h"

using namespace std;


namespace engine
{

    class LEntity;

    class LComponent
    {

        protected :

        LEntity* m_entity;

        public :

        string type;
        LComponent( LEntity* pEntity );
        ~LComponent();

        static string getStaticType();
        virtual LEntity* getEntity() { return m_entity; }

        virtual void update( float dt );

    };


}