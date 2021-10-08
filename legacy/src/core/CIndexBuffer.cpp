
#include <core/CIndexBuffer.h>

namespace engine
{
    CIndexBuffer::CIndexBuffer( const eBufferUsage& usage,
                                const uint32& count,
                                const uint32* data )
        : m_Usage( usage ), m_Count( count )
    {
        // create gl-ibo resource and send the initial data to it
        glGenBuffers( 1, &m_OpenglID );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_OpenglID );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(uint32), data, ToOpenGLEnum( m_Usage ) );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }

    CIndexBuffer::~CIndexBuffer()
    {
        glDeleteBuffers( 1, &m_OpenglID );
    }

    void CIndexBuffer::Resize( const uint32& count )
    {
        if ( m_Count == count )
            return; // no need to resize the buffer (same size requested)

        m_Count = count;
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_OpenglID );
        glBufferData( GL_ELEMENT_ARRAY_BUFFER, count * sizeof( uint32 ), NULL, ToOpenGLEnum( m_Usage ) );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }

    void CIndexBuffer::UpdateData( const uint32& count, const uint32* data )
    {
        if ( m_Count != count )
        {
            ENGINE_CORE_WARN( "CIndexBuffer::updateData >>> tried updating data for a buffer with "
                              "different size. Resizing to avoid any conflicts" );
            Resize( count );
        }

        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_OpenglID );
        glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(uint32), data );
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }

    void CIndexBuffer::Bind()
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_OpenglID );
    }

    void CIndexBuffer::Unbind()
    {
        glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
    }

    std::string CIndexBuffer::ToString() const
    {
        std::string strrep = "IndexBuffer(\n";
        strrep += "cpp-address  : " + tinyutils::PointerToHexAddress( this ) + "\n";
        strrep += "count        : " + std::to_string( m_Count ) + "\n";
        strrep += "usage        : " + engine::ToString( m_Usage ) + "\n";
        strrep += "opengl-id    : " + std::to_string( m_OpenglID ) + "\n";
        strrep += ")";
        return strrep;
    }
}
