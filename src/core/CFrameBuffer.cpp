
#include <core/CFrameBuffer.h>

namespace engine
{

    /**************************************************************************
    *                         Fbo-enums and Config impl.                      *
    ***************************************************************************/

    std::string toString( const eFboAttachment& attachment )
    {
        if ( attachment == eFboAttachment::COLOR ) return "color";
        if ( attachment == eFboAttachment::DEPTH ) return "depth";
        if ( attachment == eFboAttachment::STENCIL ) return "stencil";
        if ( attachment == eFboAttachment::DEPTH_STENCIL ) return "depth_stencil";

        ENGINE_CORE_ASSERT( false, "Invalid eFboAttachment enum given" );

        return "undefined";
    }

    uint32 toOpenGLEnum( const eFboAttachment& attachment )
    {
        if ( attachment == eFboAttachment::COLOR ) return GL_COLOR_ATTACHMENT0;
        if ( attachment == eFboAttachment::DEPTH ) return GL_DEPTH_ATTACHMENT;
        if ( attachment == eFboAttachment::STENCIL ) return GL_STENCIL_ATTACHMENT;
        if ( attachment == eFboAttachment::DEPTH_STENCIL ) return GL_DEPTH_STENCIL_ATTACHMENT;

        ENGINE_CORE_ASSERT( false, "Invalid eFboAttachment enum given" );

        return 0;
    }

    std::string toString( const CFrameBufferConfig& config )
    {
        std::string _strRep;

        _strRep += "attachment  : " + engine::toString( config.attachment ) + "\n\r";
        _strRep += "texIntFormat: " + engine::toString( config.texInternalFormat ) + "\n\r";
        _strRep += "texFormat   : " + engine::toString( config.texFormat ) + "\n\r";
        _strRep += "pixelDtype  : " + engine::toString( config.texPixelDataType ) + "\n\r";

        return _strRep;
    }

    /**************************************************************************
    *                            CFrameBuffer impl.                           *
    ***************************************************************************/

    CFrameBuffer::CFrameBuffer( const CFrameBufferConfig& config )
    {
        m_config    = config;
        m_texture   = nullptr;
        m_openglId  = 0;
        m_width     = config.width;
        m_height    = config.height;

        /* create dummy texture-data for texture attachment **********************/
        auto _textureData = new CTextureData();
        _textureData->name              = "fbo_tex";
        _textureData->data              = NULL;
        _textureData->width             = m_width;
        _textureData->height            = m_height;
        _textureData->channels          = 0; // not used for our case
        _textureData->internalFormat    = config.texInternalFormat;
        _textureData->format            = config.texFormat;

        std::shared_ptr< CTextureData > _textureDataPtr;
        _textureDataPtr.reset( _textureData );

        CTextureOptions _textureOpts;
        _textureOpts.filterMin      = eTextureFilter::NEAREST;
        _textureOpts.filterMag      = eTextureFilter::NEAREST;
        _textureOpts.wrapU          = eTextureWrap::REPEAT;
        _textureOpts.wrapV          = eTextureWrap::REPEAT;
        _textureOpts.dtype          = config.texPixelDataType;
        _textureOpts.textureUnit    = 0;

        m_texture.reset( new CTexture( _textureDataPtr, _textureOpts ) );

        /* create gl-texture resource with the given information ****************/
        glGenFramebuffers( 1, &m_openglId );
        glBindFramebuffer( GL_FRAMEBUFFER, m_openglId );

        glFramebufferTexture2D( GL_FRAMEBUFFER, 
                                toOpenGLEnum( m_config.attachment ), 
                                GL_TEXTURE_2D,
                                m_texture->openglId(),
                                0 );

        // sanity check: make sure framebuffer is correctly configured
        if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
            ENGINE_CORE_ERROR( "There seems to be an issue with a frame buffer" );

        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        /************************************************************************/
    }

    CFrameBuffer::~CFrameBuffer()
    {
        glDeleteFramebuffers( 1, &m_openglId );
    }

    void CFrameBuffer::bind()
    {
        glBindFramebuffer( GL_FRAMEBUFFER, m_openglId );
    }

    void CFrameBuffer::unbind()
    {
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }

}