
#pragma once

#include "LComponent.h"
#include "LMesh.h"

using namespace std;

namespace engine
{


    class LMeshComponent : public LComponent
    {

        private :

        LMesh* m_mesh;

        public :

        LMeshComponent( LEntity* pEntity, LMesh* pMesh );
        ~LMeshComponent();

        static string getStaticType() { return string( "mesh" ); }
        LMesh* getMesh() { return m_mesh; }

        void update( float dt ) override;

    };





}