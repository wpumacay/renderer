
#include <core/CIndexBuffer.h>
#include <utils/CLogger.h>

namespace engine
{
    CIndexBuffer::CIndexBuffer( const eBufferUsage& bufferUsage,
                                uint32 bufferCount,
                                uint32* bufferData,
                                bool track )
        : m_bufferUsage( bufferUsage ), m_bufferCount( bufferCount )
    {
        m_track = track;

        // create gl-ibo resource and send the initial data to it
        glGenBuffers( 1, &m_openglId );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_openglId );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, 
                      m_bufferCount * sizeof( uint32 ), 
                      bufferData,
                      toOpenGLEnum( m_bufferUsage) );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

        if ( m_track )
            ENGINE_CORE_TRACE( "Allocs: Created Index Buffer" );
    }

    CIndexBuffer::~CIndexBuffer()
    {
        glDeleteBuffers( 1, &m_openglId );

        if ( m_track )
        {
            if ( CLogger::IsActive() )
                ENGINE_CORE_TRACE( "Allocs: Destroyed Index Buffer" );
            else
                std::cout << "Allocs: Destroyed Index Buffer" << std::endl;
        }
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