
#include <core/CVertexBuffer.h>

namespace engine
{
    std::string ToString( const eBufferUsage& usage )
    {
        /**/ if ( usage == eBufferUsage::STATIC ) return "static";
        else if ( usage == eBufferUsage::DYNAMIC ) return "dynamic";

        ENGINE_CORE_CRITICAL( "ToString >>> Invalid eBufferUsage enum given" );
        return "undefined";
    }

    uint32 ToOpenGLEnum( const eBufferUsage& usage )
    {
        /**/ if ( usage == eBufferUsage::STATIC ) return GL_STATIC_DRAW;
        else if ( usage == eBufferUsage::DYNAMIC ) return GL_DYNAMIC_DRAW;

        ENGINE_CORE_CRITICAL( "ToOpenGLEnum >>> Invalid eBufferUsage enum given" );
        return GL_STATIC_DRAW;
    }

    CVertexBuffer::CVertexBuffer( const CVertexBufferLayout& layout,
                                  const eBufferUsage& usage,
                                  const uint32& size,
                                  const float32* data )
        : m_Layout( layout ), m_Usage( usage ), m_Size( size )
    {
        glGenBuffers( 1, &m_OpenglID );
        glBindBuffer( GL_ARRAY_BUFFER, m_OpenglID );
        glBufferData( GL_ARRAY_BUFFER, m_Size, data, ToOpenGLEnum( m_Usage ) );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );

    #if defined(ENGINE_TRACK_ALLOCS)
        if ( tinyutils::Logger::IsActive() )
            ENGINE_CORE_TRACE( "Allocs: Created Vertex Buffer" );
        else
            std::cout << "Allocs: Created Vertex Buffer" << std::endl;
    #endif
    }

    CVertexBuffer::~CVertexBuffer()
    {
        glDeleteBuffers( 1, &m_OpenglID );

    #if defined(ENGINE_TRACK_ALLOCS)
        if ( tinyutils::Logger::IsActive() )
            ENGINE_CORE_TRACE( "Allocs: Destroyed Vertex Buffer" );
        else
            std::cout << "Allocs: Destroyed Vertex Buffer" << std::endl;
    #endif
    }

    void CVertexBuffer::Resize( const uint32& size )
    {
        if ( m_Size == size )
            return; // no need to resize the buffer (same size requested)

        m_Size = size;
        glBindBuffer( GL_ARRAY_BUFFER, m_OpenglID );
        glBufferData( GL_ARRAY_BUFFER, m_Size, NULL, ToOpenGLEnum( m_Usage ) );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }

    void CVertexBuffer::UpdateData( const uint32& size, const float32* data )
    {
        if ( m_Size != size )
        {
            ENGINE_CORE_WARN( "CVertexBuffer::updateData >>> tried updating data for a buffer with \
                               different size. Resizing to avoid any conflicts" );
            Resize( size );
        }

        glBindBuffer( GL_ARRAY_BUFFER, m_OpenglID );
        glBufferSubData( GL_ARRAY_BUFFER, 0, m_Size, data );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }

    void CVertexBuffer::Bind()
    {
        glBindBuffer( GL_ARRAY_BUFFER, m_OpenglID );
    }

    void CVertexBuffer::Unbind()
    {
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }
}