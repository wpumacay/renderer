#pragma once

#include <core/CVertexBuffer.h>
#include <core/CIndexBuffer.h>

namespace engine
{

    class CVertexArray
    {

    public :
        CVertexArray();
        ~CVertexArray();

        void addVertexBuffer( CVertexBuffer* vertexBuffer );
        void setIndexBuffer( CIndexBuffer* indexBuffer );

        void bind();
        void unbind();

        std::vector< CVertexBuffer* > vertexBuffers() const { return m_vertexBuffers; }
        CIndexBuffer* indexBuffer() const { return m_indexBuffer; }
        uint32 openglId() const { return m_openglId; }

    private :

        std::vector< CVertexBuffer* >   m_vertexBuffers;
        CIndexBuffer*                   m_indexBuffer;
        uint32                          m_numAttribIndx;
        uint32                          m_openglId;

    };

}