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

    struct CAttachmentConfig
    {
        std::string     name;
        eFboAttachment  attachment;
        uint32          width;
        uint32          height;
        eTextureFormat  texInternalFormat;
        eTextureFormat  texFormat;
        ePixelDataType  texPixelDataType;

        /* extra configuration parameters for the texture-2d */
        eTextureWrap    texWrapU;
        eTextureWrap    texWrapV;
        CVec4           texBorderColorU;
        CVec4           texBorderColorV;
    };

    std::string toString( const CAttachmentConfig& config );

    class CFrameBuffer
    {

    public :

        CFrameBuffer();
        ~CFrameBuffer();

        void addAttachment( const CAttachmentConfig& config );
        void bind();
        void unbind();

        CTexture* getTextureAttachment( const std::string& name );
        CAttachmentConfig getConfigAttachment( const std::string& name );

        std::map< std::string, CTexture* > textures() const;
        std::map< std::string, CAttachmentConfig > configs() const;

        uint32 openglId() const { return m_openglId; }

    private :

        std::map< std::string, CAttachmentConfig >                  m_configs;
        std::map< std::string, std::unique_ptr< CTexture > >        m_textures;
        std::map< std::string, std::unique_ptr< CTextureData > >    m_texturesData;

        uint32 m_width;
        uint32 m_height;
        uint32 m_openglId;
    };

}