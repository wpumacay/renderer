
#pragma once

#include "../LMiniCommon.h"

#include "LMesh.h"
#include "LLightSource.h"
#include "LCamera3D.h"

using namespace std;

namespace miniengine
{


    class LScene
    {

        protected :

        vector<LMesh*> m_meshes;
        vector<LLightSource*> m_lights;

        LCamera3D* m_camera;

        public :

        LScene();
        ~LScene();

        void addObject( LMesh* pObject );
        void addLight( LLightSource* pLight );
        void render();

    };











}