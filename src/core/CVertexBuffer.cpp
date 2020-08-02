
#include <core/CVertexBuffer.h>

namespace engine
{
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
    }

    CVertexBuffer::~CVertexBuffer()
    {
        glDeleteBuffers( 1, &m_OpenglID );
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
            ENGINE_CORE_WARN( "CVertexBuffer::updateData >>> tried updating data for a buffer with "
                              "different size. Resizing to avoid any conflicts" );
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

    std::string CVertexBuffer::ToString() const
    {
        std::string strrep = "VertexBuffer(\n";
        strrep += "cpp-address  : " + tinyutils::PointerToHexAddress( this ) + "\n";
        strrep += "size         : " + std::to_string( m_Size ) + "\n";
        strrep += "usage        : " + engine::ToString( m_Usage ) + "\n";
        strrep += "opengl-id    : " + std::to_string( m_OpenglID ) + "\n";
        strrep += ")";
        return strrep;
    }
}