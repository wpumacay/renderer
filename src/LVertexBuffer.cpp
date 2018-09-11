

#include "LVertexBuffer.h"



namespace engine
{



    LVertexBuffer::LVertexBuffer()
    {
        m_usage = GL_STATIC_DRAW;
        glGenBuffers( 1, &m_bufferId );
    }

    LVertexBuffer::LVertexBuffer( GLuint usage )
    {
        m_usage = usage;
        glGenBuffers( 1, &m_bufferId );
    }

    LVertexBuffer::~LVertexBuffer()
    {
        glDeleteBuffers( 1, &m_bufferId );
    }

    void LVertexBuffer::setData( GLuint size, GLuint componentCount, GLfloat* pData )
    {
        m_componentCount = componentCount;
        glBindBuffer( GL_ARRAY_BUFFER, m_bufferId );
        glBufferData( GL_ARRAY_BUFFER, size, pData, m_usage );
    }

    void LVertexBuffer::updateData( GLuint size, GLfloat* pData )
    {
        glBindBuffer( GL_ARRAY_BUFFER, m_bufferId );
        glBufferSubData( GL_ARRAY_BUFFER, 0, size, pData );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }

    void LVertexBuffer::bind()
    {
        glBindBuffer( GL_ARRAY_BUFFER, m_bufferId );
    }

    void LVertexBuffer::unbind()
    {
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }


}