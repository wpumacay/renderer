

#include "LVertexArray.h"




namespace engine
{


    LVertexArray::LVertexArray()
    {
        glGenVertexArrays( 1, &m_vertexArrayId );
    }

    LVertexArray::~LVertexArray()
    {
        for ( LVertexBuffer* pBuffer : m_vBuffers )
        {
            delete pBuffer;
        }

        m_vBuffers.clear();
    }

    void LVertexArray::addBuffer( LVertexBuffer* pBuffer, GLuint attribIndex )
    {
        m_vBuffers.push_back( pBuffer );

        bind();

        pBuffer->bind();

        glEnableVertexAttribArray( attribIndex );
        glVertexAttribPointer( attribIndex, pBuffer->getComponentCount(), GL_FLOAT, GL_FALSE, 0, 0 );

        pBuffer->unbind();

        unbind();
    }

    void LVertexArray::bind()
    {
        glBindVertexArray( m_vertexArrayId );
    }

    void LVertexArray::unbind()
    {
        glBindVertexArray( 0 );
    }

}