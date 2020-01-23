
#include <core/CVertexBuffer.h>
#include <utils/CLogger.h>

namespace engine
{

    std::string toString( const eBufferUsage& usage )
    {
        if ( usage == eBufferUsage::STATIC ) return "static";
        if ( usage == eBufferUsage::DYNAMIC ) return "dynamic";

        ENGINE_CORE_ASSERT( false, "Invalid usage enum given" );
        return "undefined";
    }

    uint32 toOpenGLEnum( const eBufferUsage& usage )
    {
        if ( usage == eBufferUsage::STATIC ) return GL_STATIC_DRAW;
        if ( usage == eBufferUsage::DYNAMIC ) return GL_DYNAMIC_DRAW;

        ENGINE_CORE_ASSERT( false, "Invalid usage enum given" );
        return GL_STATIC_DRAW;
    }


    CVertexBuffer::CVertexBuffer( const CVertexBufferLayout& bufferLayout, 
                                  const eBufferUsage& bufferUsage, 
                                  uint32 bufferSize, 
                                  float32* bufferData,
                                  bool track )
        : m_bufferLayout( bufferLayout ), m_bufferUsage( bufferUsage ), m_bufferSize( bufferSize )
    {
        m_track = track;

        // create gl-vbo resource and send the initial data to it
        glGenBuffers( 1, &m_openglId );
        glBindBuffer( GL_ARRAY_BUFFER, m_openglId );
        glBufferData( GL_ARRAY_BUFFER, m_bufferSize, bufferData, toOpenGLEnum( m_bufferUsage ) );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        if ( m_track )
            ENGINE_CORE_TRACE( "Allocs: Created Vertex Buffer" );
    }

    CVertexBuffer::~CVertexBuffer()
    {
        glDeleteBuffers( 1, &m_openglId );

        if ( m_track )
            ENGINE_CORE_TRACE( "Allocs: Released Vertex Buffer" );
    }

    void CVertexBuffer::updateData( uint32 dataSize, float32* dataPtr )
    {
        glBindBuffer( GL_ARRAY_BUFFER, m_openglId );
        glBufferSubData( GL_ARRAY_BUFFER, 0, dataSize, dataPtr );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }

    void CVertexBuffer::bind()
    {
        glBindBuffer( GL_ARRAY_BUFFER, m_openglId );
    }

    void CVertexBuffer::unbind()
    {
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }

}