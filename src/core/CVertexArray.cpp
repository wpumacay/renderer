
#include <core/CVertexArray.h>
#include <utils/CLogger.h>

namespace engine
{

    CVertexArray::CVertexArray( bool track )
    {
        m_numAttribIndx = 0;
        m_openglId      = 0;
        m_indexBuffer   = nullptr;
        m_track = track;

        glGenVertexArrays( 1, &m_openglId );

        if ( m_track )
            ENGINE_CORE_TRACE( "Allocs: Created Vertex Array Object" );
    }

    CVertexArray::~CVertexArray()
    {
        // released owned VBOs
        m_vertexBuffers.clear();
        // release owned EBO
        m_indexBuffer = nullptr;

        glDeleteVertexArrays( 1, &m_openglId );

        if ( m_track )
        {
            if ( CLogger::IsActive() )
                ENGINE_CORE_TRACE( "Allocs: Destroyed Vertex Array Object" );
            else // in python-land (when exiting script) we might have destroyed logger first
                std::cout << "Allocs: Destroyed Vertex Array Object" << std::endl;
        }
    }

    void CVertexArray::addVertexBuffer( std::unique_ptr<CVertexBuffer> vertexBuffer, bool isInstanced )
    {
        // setup the VBO within our VAO according to its layout
        auto _buffLayout = vertexBuffer->layout();
        auto _buffElements = _buffLayout.elements();

        bind();
        vertexBuffer->bind();

        for ( size_t i = 0; i < _buffElements.size(); i++ )
        {
            glEnableVertexAttribArray( m_numAttribIndx );
            glVertexAttribPointer( m_numAttribIndx,
                                   _buffElements[i].count,
                                   toOpenGLType( _buffElements[i].type ),
                                   _buffElements[i].normalized ? GL_TRUE : GL_FALSE,
                                   _buffLayout.stride(),
                                   (const void*) (intptr_t) _buffElements[i].offset );
            if ( isInstanced )
                glVertexAttribDivisor( m_numAttribIndx, 1 );

            m_numAttribIndx++;
        }

        vertexBuffer->unbind();
        unbind();

        // keep a reference only
        // keep a reference only
        m_vertexBuffers.push_back( std::move( vertexBuffer ) );
    }

    void CVertexArray::setIndexBuffer( std::unique_ptr<CIndexBuffer> indexBuffer )
    {
        // setup the IBO within our VAO
        glBindVertexArray( m_openglId );
        indexBuffer->bind();
        glBindVertexArray( 0 );
        indexBuffer->unbind();

        // keep a reference only
        m_indexBuffer = std::move( indexBuffer );
    }

    void CVertexArray::bind()
    {
        glBindVertexArray( m_openglId );
    }

    void CVertexArray::unbind()
    {
        glBindVertexArray( 0 );
    }

}