
#include <core/CVertexArray.h>


namespace engine
{

    CVertexArray::CVertexArray()
    {
        m_numAttribIndx = 0;
        m_openglId      = 0;
        m_indexBuffer   = nullptr;

        glGenVertexArrays( 1, &m_openglId );
    }

    CVertexArray::~CVertexArray()
    {
        glDeleteVertexArrays( 1, &m_openglId );

        m_indexBuffer = nullptr;
        m_vertexBuffers.clear();
    }

    void CVertexArray::addVertexBuffer( CVertexBuffer* vertexBuffer, bool isInstanced )
    {
        // setup the VBO within our VAO according to its layout
        auto _buffLayout = vertexBuffer->layout();
        auto _buffElements = _buffLayout.elements();

        bind();
        vertexBuffer->bind();

        for ( size_t i = 0; i < _buffElements.size(); i++ )
        {
            glEnableVertexAttribArray( m_numAttribIndx );
            glVertexAttribPointer( m_numAttribIndx,
                                   _buffElements[i].count,
                                   toOpenGLType( _buffElements[i].type ),
                                   _buffElements[i].normalized ? GL_TRUE : GL_FALSE,
                                   _buffLayout.stride(),
                                   (const void*) (intptr_t) _buffElements[i].offset );
            if ( isInstanced )
                glVertexAttribDivisor( m_numAttribIndx, 1 );

            m_numAttribIndx++;
        }

        vertexBuffer->unbind();
        unbind();

        // keep a reference only
        m_vertexBuffers.push_back( vertexBuffer );
    }

    void CVertexArray::setIndexBuffer( CIndexBuffer* indexBuffer )
    {
        // setup the IBO within our VAO
        glBindVertexArray( m_openglId );
        indexBuffer->bind();
        glBindVertexArray( 0 );
        indexBuffer->unbind();

        // keep a reference only
        m_indexBuffer = indexBuffer;
    }

    void CVertexArray::bind()
    {
        glBindVertexArray( m_openglId );
    }

    void CVertexArray::unbind()
    {
        glBindVertexArray( 0 );
    }

}