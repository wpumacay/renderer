#pragma once

#include <CCommon.h>
#include <CMath.h>

#include <core/CVertexBuffer.h>
#include <core/CVertexArray.h>
#include <core/CTextureCube.h>
#include <assets/CTextureManager.h>

namespace engine
{

    class CSkybox
    {

    public :

        CSkybox();
        ~CSkybox();

        void render();
        void setCubemap( CTextureCube* cubemap ) { m_cubemap = cubemap; }

        CVertexArray* vao() const { return m_vertexArray.get(); }
        CTextureCube* cubemap() const { return m_cubemap; }

    private :

        CTextureCube*                       m_cubemap;
        std::unique_ptr< CVertexBuffer >    m_vertexBuffer;
        std::unique_ptr< CVertexArray >     m_vertexArray;

    };

}