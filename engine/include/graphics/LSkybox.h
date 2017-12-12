

#pragma once

#include "../LCommon.h"
#include "../gl/LVertexBuffer.h"
#include "../gl/LVertexArray.h"
#include "../gl/LCubeTexture.h"

using namespace std;


namespace engine
{


    class LSkybox
    {

        private : 

        LVertexArray* m_vertexArray;
        LCubeTexture* m_cubeTexture;

        public :

        LSkybox( string skyboxResId );
        ~LSkybox();

        void render();

        LCubeTexture* getCubeTexture() { return m_cubeTexture; }

    };






}