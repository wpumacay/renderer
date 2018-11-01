

#include <LIndexBuffer.h>

namespace engine
{


    LIndexBuffer::LIndexBuffer()
    {
        m_usage = GL_STATIC_DRAW;
        glGenBuffers( 1, &m_bufferId );
    }

    LIndexBuffer::LIndexBuffer( GLuint usage )
    {
        m_usage = usage;
        glGenBuffers( 1, &m_bufferId );
    }

    LIndexBuffer::~LIndexBuffer()
    {
        glDeleteBuffers( 1, &m_bufferId );
    }

    void LIndexBuffer::setData( GLuint size, GLuint count, GLuint* pData )
    {
        m_size = size;
        m_count = count;

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_bufferId );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, size, pData, m_usage );
    }

    void LIndexBuffer::bind()
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_bufferId );
    }

    void LIndexBuffer::unbind()
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }


}