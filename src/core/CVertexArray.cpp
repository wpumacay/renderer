
#include <core/CVertexArray.h>


namespace engine
{

    CVertexArray::CVertexArray()
    {
        m_numAttribIndx = 0;
        m_openglId      = 0;
        m_indexBuffer   = NULL;

        glGenVertexArrays( 1, &m_openglId );
    }

    CVertexArray::~CVertexArray()
    {
        for ( size_t i = 0; i < m_vertexBuffers.size(); i++ )
            delete m_vertexBuffers[i];
        m_vertexBuffers.clear();

        delete m_indexBuffer;
        m_indexBuffer = NULL;
    }

    void CVertexArray::addVertexBuffer( CVertexBuffer* vertexBuffer )
    {
        m_vertexBuffers.push_back( vertexBuffer );

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

            m_numAttribIndx++;
        }

        vertexBuffer->unbind();
        unbind();
    }

    void CVertexArray::setIndexBuffer( CIndexBuffer* indexBuffer )
    {
        m_indexBuffer = indexBuffer;

        glBindVertexArray( m_openglId );
        m_indexBuffer->bind();
        glBindVertexArray( 0 );
        m_indexBuffer->unbind();
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