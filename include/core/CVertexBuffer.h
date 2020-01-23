#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>
#include <core/CVertexBufferLayout.h>

namespace engine
{

    enum class eBufferUsage
    {
        STATIC = 0, DYNAMIC
    };

    std::string toString( const eBufferUsage& usage );

    uint32 toOpenGLEnum( const eBufferUsage& usage );

    class CVertexBuffer
    {

    public :

        CVertexBuffer( const CVertexBufferLayout& bufferLayout, 
                       const eBufferUsage& bufferUsage, 
                       uint32 bufferSize, 
                       float32* bufferData,
                       bool track = false );

        ~CVertexBuffer();

        void updateData( uint32 bufferSize, float32* bufferData );
        void bind();
        void unbind();

        CVertexBufferLayout layout() const { return m_bufferLayout; }
        uint32 size() const { return m_bufferSize; }
        eBufferUsage usage() const { return m_bufferUsage; }
        uint32 openglId() const { return m_openglId; }

    private :

        CVertexBufferLayout m_bufferLayout;
        eBufferUsage        m_bufferUsage;
        uint32              m_bufferSize;
        uint32              m_openglId;
        bool                m_track;

    };

}