
#include <core/CVertexArray.h>

namespace engine
{
    CVertexArray::CVertexArray()
    {
        glGenVertexArrays( 1, &m_OpenglID );
    }

    CVertexArray::~CVertexArray()
    {
        m_VertexBuffers.clear();
        m_IndexBuffer = nullptr;

        glDeleteVertexArrays( 1, &m_OpenglID );
    }

    void CVertexArray::AddVertexBuffer( std::unique_ptr<CVertexBuffer> vertexBuffer, bool isInstanced )
    {
        auto buffer_layout = vertexBuffer->layout();
        auto buffer_elements = buffer_layout.elements();

        Bind();
        vertexBuffer->Bind();

        for ( const auto& buffer_element : buffer_elements )
        {
            glEnableVertexAttribArray( m_NumAttribIndx );
            glVertexAttribPointer( m_NumAttribIndx,
                                   buffer_element.count,
                                   ToOpenGLType( buffer_element.type ),
                                   buffer_element.normalized ? GL_TRUE : GL_FALSE,
                                   buffer_layout.stride(),
                                   (const void*) (intptr_t) buffer_element.offset );
            if ( isInstanced )
                glVertexAttribDivisor( m_NumAttribIndx, 1 );

            m_NumAttribIndx++;
        }

        vertexBuffer->Unbind();
        Unbind();

        m_VertexBuffers.push_back( std::move( vertexBuffer ) );
    }

    void CVertexArray::SetIndexBuffer( std::unique_ptr<CIndexBuffer> indexBuffer )
    {
        glBindVertexArray( m_OpenglID );
        indexBuffer->Bind();
        glBindVertexArray( 0 );
        indexBuffer->Unbind();

        m_IndexBuffer = std::move( indexBuffer );
    }

    void CVertexArray::Bind()
    {
        glBindVertexArray( m_OpenglID );
    }

    void CVertexArray::Unbind()
    {
        glBindVertexArray( 0 );
    }
}
