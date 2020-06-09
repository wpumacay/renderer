
#include <core/CIndexBuffer.h>

namespace engine
{
    CIndexBuffer::CIndexBuffer( const eBufferUsage& bufferUsage,
                                uint32 bufferCount,
                                uint32* bufferData )
        : m_bufferUsage( bufferUsage ), m_bufferCount( bufferCount )
    {
        // create gl-ibo resource and send the initial data to it
        glGenBuffers( 1, &m_openglId );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_openglId );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, 
                      m_bufferCount * sizeof( uint32 ), 
                      bufferData,
                      toOpenGLEnum( m_bufferUsage) );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

        ENGINE_CORE_TRACE( "Allocs: Created Index Buffer" );
    }

    CIndexBuffer::~CIndexBuffer()
    {
        glDeleteBuffers( 1, &m_openglId );

        if ( tinyutils::Logger::IsActive() )
            ENGINE_CORE_TRACE( "Allocs: Destroyed Index Buffer" );
        else
            std::cout << "Allocs: Destroyed Index Buffer" << std::endl;
    }

    void CIndexBuffer::resize( uint32 bufferCount )
    {
        if ( m_bufferCount == bufferCount )
            return; // no need to resize the buffer (same size requested)

        m_bufferCount = bufferCount;
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_openglId );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, bufferCount * sizeof( uint32 ), NULL, toOpenGLEnum( m_bufferUsage ) );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }

    void CIndexBuffer::updateData( uint32 bufferCount, uint32* bufferData )
    {
        if ( bufferCount != m_bufferCount )
        {
            ENGINE_CORE_WARN( "CIndexBuffer::updateData >>> tried updating data for a buffer with \
                              different size. Resizing to avoid any conflicts" );
            resize( bufferCount );
        }

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