
#include <core/CVertexBuffer.h>

namespace engine
{

    CVertexBuffer::CVertexBuffer( uint32 bufferSize, 
                                  uint32 bufferComponentCount, 
                                  uint32 bufferUsage, 
                                  float32* bufferData )
    {
        m_bufferSize            = bufferSize;
        m_bufferComponentCount  = bufferComponentCount;
        m_bufferUsage           = bufferUsage;
    }

    CVertexBuffer::~CVertexBuffer()
    {

    }

    void CVertexBuffer::updateData( uint32 bufferSize, float32* bufferData )
    {
        
    }

    void CVertexBuffer::updateData( uint32 bufferSize, uint32 bufferOffset, float32* bufferData )
    {

    }

    void CVertexBuffer::bind()
    {

    }

    void CVertexBuffer::unbind()
    {

    }

}