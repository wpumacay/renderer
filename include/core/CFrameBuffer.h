#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <core/CTexture.h>

namespace engine
{

    enum class eFboAttachment
    {
        NONE = 0,
        COLOR, DEPTH, STENCIL, DEPTH_STENCIL
    };

    std::string toString( const eFboAttachment& attachment );
    uint32 toOpenGLEnum( const eFboAttachment& attachment );

    struct CFrameBufferConfig
    {
        eFboAttachment  attachment;
        uint32          width;
        uint32          height;
        eTextureFormat  texInternalFormat;
        eTextureFormat  texFormat;
        ePixelDataType  texPixelDataType;
    };

    std::string toString( const CFrameBufferConfig& config );

    class CFrameBuffer
    {

    public :

        CFrameBuffer( const CFrameBufferConfig& config );
        ~CFrameBuffer();

        void bind();
        void unbind();

        std::shared_ptr< CTexture > texture() const { return m_texture; }
        CFrameBufferConfig config() const { return m_config; }
        uint32 openglId() const { return m_openglId; }

    private :

        std::shared_ptr< CTexture >     m_texture;
        CFrameBufferConfig              m_config;
        uint32                          m_width;
        uint32                          m_height;
        uint32                          m_openglId;

    };

}