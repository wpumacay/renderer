#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <core/CVertexBuffer.h>
#include <core/CVertexArray.h>
#include <LCubeTexture.h>
#include <LAssetsManager.h>

using namespace std;

namespace engine
{

    class LSkybox
    {
        private : 

        CVertexArray* m_vertexArray;
        LCubeTexture* m_cubeTextureRef;

        public :

        LSkybox( const string& skyboxResId );
        ~LSkybox();

        void render();
    };

}