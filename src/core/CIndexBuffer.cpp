
#include <core/CIndexBuffer.h>

namespace engine
{
    CIndexBuffer::CIndexBuffer( const eBufferUsage& bufferUsage,
                                uint32 bufferCount,
                                uint32* bufferData )
    {
        m_bufferUsage = bufferUsage;
        m_bufferCount = bufferCount;

        // create gl-ibo resource and send the initial data to it
        glGenBuffers( 1, &m_openglId );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_openglId );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, 
                      m_bufferCount * sizeof( uint32 ), 
                      bufferData,
                      toOpenGLEnum( m_bufferUsage) );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }

    void CIndexBuffer::updateData( uint32 bufferCount, uint32* bufferData )
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_openglId );
        glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0, bufferCount * sizeof( uint32 ), bufferData );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }

    void CIndexBuffer::bind()
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_openglId );
    }

    void CIndexBuffer::unbind()
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }
}