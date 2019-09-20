#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>

namespace engine
{

    class CVertexBuffer
    {

    public :

        CVertexBuffer( uint32 bufferSize, 
                       uint32 bufferComponentCount, 
                       uint32 bufferUsage, 
                       float32* bufferData );

        ~CVertexBuffer();

        void updateData( uint32 bufferSize, float32* bufferData );
        void updateData( uint32 bufferSize, uint32 bufferOffset, float32* bufferData );
        void bind();
        void unbind();

        uint32 componentCount() { return m_bufferComponentCount; }
        uint32 size() { return m_bufferSize; }
        uint32 usage() { return m_bufferUsage; }

    private :

        uint32 m_bufferSize;
        uint32 m_bufferComponentCount;
        uint32 m_bufferUsage;
        uint32 m_bufferOpenGLId;

    };

}