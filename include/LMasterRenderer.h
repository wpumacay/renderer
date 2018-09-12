
#pragma once

#include "LMeshRenderer.h"

namespace engine
{


    class LMasterRenderer
    {

        private :

        LMeshRenderer* m_meshRenderer;

        static LMasterRenderer* _INSTANCE;
        LMasterRenderer();

        public :

        static LMasterRenderer* GetInstance();
        ~LMasterRenderer();

        void render( LScene* pScene );

    };



}