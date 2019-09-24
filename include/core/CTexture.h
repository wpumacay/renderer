#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>

namespace engine
{

    enum class eTextureFormat
    {
        NONE = 0, 
        RGB, 
        RGBA, 
        DEPTH, 
        STENCIL, 
        DEPTH24_STENCIL8
    };

    std::string toString( const eTextureFormat& format );
    uint32 toOpenGLEnum( const eTextureFormat& format );

    enum class eTextureWrap
    {
        NONE = 0, 
        REPEAT, 
        REPEAT_MIRROR, 
        CLAMP_TO_EDGE, 
        CLAMP_TO_BORDER
    };

    std::string toString( const eTextureWrap& wrap );
    uint32 toOpenGLEnum( const eTextureWrap& wrap );

    enum class eTextureFilter
    {
        NONE = 0,
        NEAREST,
        LINEAR
    };

    std::string toString( const eTextureFilter& filter );
    uint32 toOpenGLEnum( const eTextureFilter& filter );

    struct CTextureData
    {
        std::string         name;
        uint8*              data;
        int                 width;
        int                 height;
        int                 channels;
        eTextureFormat      internalFormat;
        eTextureFormat      format;

        CTextureData();
        ~CTextureData();
    };

    std::string toString( const CTextureData& texData );

    struct CTextureOptions
    {
        eTextureFilter  filterMin;
        eTextureFilter  filterMag;
        eTextureWrap    wrapU;
        eTextureWrap    wrapV;
        CVec4           borderColorU;
        CVec4           borderColorV;
        ePixelDataType  dtype;
        uint32          textureUnit;
    };

    class CTexture
    {

    public :

        CTexture( std::shared_ptr< CTextureData > texData,
                  const eTextureFilter& filterMin,
                  const eTextureFilter& filterMag,
                  const eTextureWrap& wrapU,
                  const eTextureWrap& wrapV,
                  const CVec4& borderColorU,
                  const CVec4& borderColorV,
                  const ePixelDataType& dtype,
                  uint32 textureUnit );

        CTexture( std::shared_ptr< CTextureData > texData,
                  const CTextureOptions& texOptions );

        ~CTexture();

        void bind();
        void unbind();

        std::shared_ptr< CTextureData > data() const { return m_texData; }

        std::string name() const { return m_texData->name; }
        uint32 width() const { return m_texData->width; }
        uint32 height() const { return m_texData->height; }
        uint32 channels() const { return m_texData->channels; }
        eTextureFormat format() const { return m_texData->format; }

        eTextureFilter filterMin() const { return m_texFilterModeMin; }
        eTextureFilter filterMag() const { return m_texFilterModeMag; }
        eTextureWrap wrapU() const { return m_texWrapModeU; }
        eTextureWrap wrapV() const { return m_texWrapModeV; }
        CVec4 borderColorU() const { return m_texBorderColorU; }
        CVec4 borderColorV() const { return m_texBorderColorV; }
        ePixelDataType dtype() const { return m_texPixelDtype; }

        uint32 openglId() const { return m_openglId; }
        uint32 openglTexUnit() const { return m_openglTexUnit; }

    private :

        std::shared_ptr< CTextureData > m_texData;
        eTextureWrap                    m_texWrapModeU;
        eTextureWrap                    m_texWrapModeV;
        eTextureFilter                  m_texFilterModeMin;
        eTextureFilter                  m_texFilterModeMag;
        CVec4                           m_texBorderColorU;
        CVec4                           m_texBorderColorV;
        ePixelDataType                  m_texPixelDtype;
        uint32                          m_openglTexUnit;
        uint32                          m_openglId;
    };

}