#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <core/CVertexBuffer.h>
#include <core/CVertexArray.h>
#include <core/CTextureCube.h>
#include <assets/CTextureManager.h>

namespace engine
{

    class LSkybox
    {
        private : 

        CVertexArray*   m_vertexArray;
        CTextureCube*   m_textureCube;

        public :

        LSkybox( const std::string& skyboxResId );
        ~LSkybox();

        void render();
    };

}