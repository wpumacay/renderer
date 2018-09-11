#pragma once

#include "LCommon.h"
#include "LVertexBuffer.h"
#include "LVertexArray.h"
#include "LCubeTexture.h"

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