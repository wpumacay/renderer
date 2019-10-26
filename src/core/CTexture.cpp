
#include <core/CTexture.h>

namespace engine
{

    /**************************************************************************
    *                 Enums, CTextureOptions and CTextureData impl.           *
    ***************************************************************************/

    std::string toString( const eTextureFormat& format )
    {
        if ( format == eTextureFormat::RGB ) return "rgb";
        if ( format == eTextureFormat::RGBA ) return "rgba";
        if ( format == eTextureFormat::DEPTH ) return "depth_component";
        if ( format == eTextureFormat::STENCIL ) return "stencil_index";
        if ( format == eTextureFormat::DEPTH24_STENCIL8 ) return "depth24_stencil8";

        ENGINE_CORE_ASSERT( false, "Invalid eTextureFormat enum given" );

        return "undefined";
    }

    uint32 toOpenGLEnum( const eTextureFormat& format )
    {
        if ( format == eTextureFormat::RGB ) return GL_RGB;
        if ( format == eTextureFormat::RGBA ) return GL_RGBA;
        if ( format == eTextureFormat::DEPTH ) return GL_DEPTH_COMPONENT;
        if ( format == eTextureFormat::STENCIL ) return GL_STENCIL_INDEX;
        if ( format == eTextureFormat::DEPTH24_STENCIL8 ) return GL_DEPTH24_STENCIL8;

        ENGINE_CORE_ASSERT( false, "Invalid eTextureFormat enum given" );

        return 0;
    }

    std::string toString( const eTextureWrap& wrap )
    {
        if ( wrap == eTextureWrap::REPEAT ) return "repeat";
        if ( wrap == eTextureWrap::REPEAT_MIRROR ) return "repeat_mirror";
        if ( wrap == eTextureWrap::CLAMP_TO_EDGE ) return "clamp_to_edge";
        if ( wrap == eTextureWrap::CLAMP_TO_BORDER ) return "clamp_to_border";

        ENGINE_CORE_ASSERT( false, "Invalid eTextureWrap enum given" );

        return "undefined";
    }

    uint32 toOpenGLEnum( const eTextureWrap& wrap )
    {
        if ( wrap == eTextureWrap::REPEAT ) return GL_REPEAT;
        if ( wrap == eTextureWrap::REPEAT_MIRROR ) return GL_MIRRORED_REPEAT;
        if ( wrap == eTextureWrap::CLAMP_TO_EDGE ) return GL_CLAMP_TO_EDGE;
        if ( wrap == eTextureWrap::CLAMP_TO_BORDER ) return GL_CLAMP_TO_BORDER;

        ENGINE_CORE_ASSERT( false, "Invalid eTextureWrap enum given" );

        return 0;
    }

    std::string toString( const eTextureFilter& filter )
    {
        if ( filter == eTextureFilter::NEAREST ) return "nearest";
        if ( filter == eTextureFilter::LINEAR ) return "linear";

        ENGINE_CORE_ASSERT( false, "Invalid eTextureFilter enum given" );

        return "undefined";
    }

    uint32 toOpenGLEnum( const eTextureFilter& filter )
    {
        if ( filter == eTextureFilter::NEAREST ) return GL_NEAREST;
        if ( filter == eTextureFilter::LINEAR ) return GL_LINEAR;

        ENGINE_CORE_ASSERT( false, "Invalid eTextureFilter enum given" );

        return 0;
    }

    CTextureData::CTextureData()
    {
        name            = "undefined";
        data            = NULL;
        width           = 0;
        height          = 0;
        channels        = 0;
        internalFormat  = eTextureFormat::NONE;
        format          = eTextureFormat::NONE;
    }

    CTextureData::~CTextureData()
    {
        if ( data )
            delete data;

        data = NULL;
    }

    std::string toString( const CTextureData& texData )
    {
        std::string _strRep;

        _strRep += "name    : " + texData.name + "\n\r";
        _strRep += "width   : " + std::to_string( texData.width ) + "\n\r";
        _strRep += "height  : " + std::to_string( texData.height ) + "\n\r";
        _strRep += "chnls   : " + std::to_string( texData.channels ) + "\n\r";
        _strRep += "iformat.: " + engine::toString( texData.internalFormat ) + "\n\r";
        _strRep += "format  : " + engine::toString( texData.format ) + "\n\r";

        return _strRep;
    }

    /**************************************************************************
    *                             CTexture impl.                              *
    ***************************************************************************/

    CTexture::CTexture( CTextureData* texData,
                        const eTextureFilter& filterMin,
                        const eTextureFilter& filterMag,
                        const eTextureWrap& wrapU,
                        const eTextureWrap& wrapV,
                        const CVec4& borderColorU,
                        const CVec4& borderColorV,
                        const ePixelDataType& dtype )
    {
        m_texDataPtr        = texData;
        m_texWrapModeU      = wrapU;
        m_texWrapModeV      = wrapV;
        m_texFilterModeMin  = filterMin;
        m_texFilterModeMag  = filterMag;
        m_texBorderColorU   = borderColorU;
        m_texBorderColorV   = borderColorV;
        m_texPixelDtype     = dtype;
        m_openglId          = 0;

        /* create gl-texture resource with the given information ****************/
        glGenTextures( 1, &m_openglId );
        glBindTexture( GL_TEXTURE_2D, m_openglId );

        /* configure wrapping mode */
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, toOpenGLEnum( m_texWrapModeU ) );
        if ( m_texWrapModeU == eTextureWrap::CLAMP_TO_BORDER )
        {
            float32 _color[] = { m_texBorderColorU.x, m_texBorderColorU.y, m_texBorderColorU.z, m_texBorderColorU.w };
            glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, _color );
        }

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, toOpenGLEnum( m_texWrapModeV ) );
        if ( m_texWrapModeV == eTextureWrap::CLAMP_TO_BORDER )
        {
            float32 _color[] = { m_texBorderColorV.x, m_texBorderColorV.y, m_texBorderColorV.z, m_texBorderColorV.w };
            glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, _color );
        }

        /* configure min-mag filtering mode */
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toOpenGLEnum( m_texFilterModeMin ) );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toOpenGLEnum( m_texFilterModeMag ) );

        /* send our data to the texture buffer */
        glTexImage2D( GL_TEXTURE_2D, 0, 
                      toOpenGLEnum( m_texDataPtr->internalFormat ), m_texDataPtr->width, m_texDataPtr->height, 0,
                      toOpenGLEnum( m_texDataPtr->format ), toOpenGLEnum( m_texPixelDtype ), m_texDataPtr->data );

        // create mipmaps for textures loaded from disk, not for potential fbo attachments
        if ( m_texDataPtr->data )
            glGenerateMipmap( GL_TEXTURE_2D );

        glBindTexture( GL_TEXTURE_2D, 0 );
        /***********************************************************************/
    }

    CTexture::CTexture( CTextureData* texData,
                        const CTextureOptions& texOptions )
        : CTexture( texData,
                    texOptions.filterMin,
                    texOptions.filterMag,
                    texOptions.wrapU,
                    texOptions.wrapV,
                    texOptions.borderColorU,
                    texOptions.borderColorV,
                    texOptions.dtype )
    {
        // no additional construction steps needed
    }

    CTexture::~CTexture()
    {
        if ( m_openglId != 0 )
            glDeleteTextures( 1, &m_openglId );

        m_openglId = 0;
    }

    void CTexture::bind()
    {
        glBindTexture( GL_TEXTURE_2D, m_openglId );
    }

    void CTexture::unbind()
    {
        glBindTexture( GL_TEXTURE_2D, 0 );
    }

    void CTexture::resize( int32 width, int32 height )
    {
        if ( m_texDataPtr->data )
        {
            ENGINE_CORE_WARN( "Tried resizing a texture associated to non-null initial texture data. Skipping resize" );
            return;
        }

        m_texDataPtr->width = width;
        m_texDataPtr->height = height;

        glBindTexture( GL_TEXTURE_2D, m_openglId );
        glTexImage2D( GL_TEXTURE_2D, 0,
                      toOpenGLEnum( m_texDataPtr->internalFormat ), width, height, 0,
                      toOpenGLEnum( m_texDataPtr->format ), toOpenGLEnum( m_texPixelDtype ), NULL );
        glBindTexture( GL_TEXTURE_2D, 0 );
    }

}