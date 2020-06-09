
#include <core/CVertexBuffer.h>

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
                                  float32* bufferData )
        : m_bufferLayout( bufferLayout ), m_bufferUsage( bufferUsage ), m_bufferSize( bufferSize )
    {
        // create gl-vbo resource and send the initial data to it
        glGenBuffers( 1, &m_openglId );
        glBindBuffer( GL_ARRAY_BUFFER, m_openglId );
        glBufferData( GL_ARRAY_BUFFER, m_bufferSize, bufferData, toOpenGLEnum( m_bufferUsage ) );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );

        if ( tinyutils::Logger::IsActive() )
            ENGINE_CORE_TRACE( "Allocs: Created Vertex Buffer" );
        else
            std::cout << "Allocs: Created Vertex Buffer" << std::endl;
    }

    CVertexBuffer::~CVertexBuffer()
    {
        glDeleteBuffers( 1, &m_openglId );

        if ( tinyutils::Logger::IsActive() )
            ENGINE_CORE_TRACE( "Allocs: Destroyed Vertex Buffer" );
        else
            std::cout << "Allocs: Destroyed Vertex Buffer" << std::endl;
    }

    void CVertexBuffer::resize( uint32 bufferSize )
    {
        if ( m_bufferSize == bufferSize )
            return; // no need to resize the buffer (same size requested)

        m_bufferSize = bufferSize;
        glBindBuffer( GL_ARRAY_BUFFER, m_openglId );
        glBufferData( GL_ARRAY_BUFFER, bufferSize, NULL, toOpenGLEnum( m_bufferUsage ) );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }

    void CVertexBuffer::updateData( uint32 dataSize, float32* dataPtr )
    {
        if ( dataSize != m_bufferSize )
        {
            ENGINE_CORE_WARN( "CVertexBuffer::updateData >>> tried updating data for a buffer with \
                              different size. Resizing to avoid any conflicts" );
            resize( dataSize );
        }

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