#pragma once

#include <LCommon.h>
#include <LVertexBuffer.h>
#include <LVertexArray.h>
#include <LCubeTexture.h>

using namespace std;

namespace engine
{


    class LSkybox
    {
        private : 

        LVertexArray* m_vertexArray;
        LCubeTexture* m_cubeTextureRef;

        public :

        LSkybox( const string& skyboxResId );
        ~LSkybox();

        void render();
    };






}