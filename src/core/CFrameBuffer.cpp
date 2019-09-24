
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

    std::string toString( const CAttachmentConfig& config )
    {
        std::string _strRep;

        _strRep += "name            : " + config.name + "\n\r";
        _strRep += "attachment      : " + engine::toString( config.attachment ) + "\n\r";
        _strRep += "texIntFormat    : " + engine::toString( config.texInternalFormat ) + "\n\r";
        _strRep += "texFormat       : " + engine::toString( config.texFormat ) + "\n\r";
        _strRep += "texPixelDtype   : " + engine::toString( config.texPixelDataType ) + "\n\r";
        _strRep += "texWrapU        : " + engine::toString( config.texWrapU ) + "\n\r";
        _strRep += "texWrapV        : " + engine::toString( config.texWrapV ) + "\n\r";
        _strRep += "texBorderColorU : " + engine::toString( config.texBorderColorU ) + "\n\r";
        _strRep += "texBorderColorV : " + engine::toString( config.texBorderColorV ) + "\n\r";

        return _strRep;
    }

    /**************************************************************************
    *                            CFrameBuffer impl.                           *
    ***************************************************************************/

    CFrameBuffer::CFrameBuffer()
    {
        // create gl-texture resource (will configure it later)
        glGenFramebuffers( 1, &m_openglId );
    }

    CFrameBuffer::~CFrameBuffer()
    {
        glDeleteFramebuffers( 1, &m_openglId );

        m_textures.clear();
        m_configs.clear();
    }

    void CFrameBuffer::addAttachment( const CAttachmentConfig& config )
    {
        m_configs[config.name] = config;

        // bind our current framebuffer object
        glBindFramebuffer( GL_FRAMEBUFFER, m_openglId );

        /* create dummy texture-data for texture attachment **********************/
        auto _textureData = new CTextureData();
        _textureData->name              = "fbo_" + config.name;
        _textureData->data              = NULL;
        _textureData->width             = config.width;
        _textureData->height            = config.height;
        _textureData->channels          = 0; // not used for our case
        _textureData->internalFormat    = config.texInternalFormat;
        _textureData->format            = config.texFormat;

        std::shared_ptr< CTextureData > _textureDataPtr;
        _textureDataPtr.reset( _textureData );

        CTextureOptions _textureOpts;
        _textureOpts.filterMin      = eTextureFilter::NEAREST;
        _textureOpts.filterMag      = eTextureFilter::NEAREST;
        _textureOpts.wrapU          = config.texWrapU;
        _textureOpts.wrapV          = config.texWrapV;
        _textureOpts.borderColorU   = config.texBorderColorU;
        _textureOpts.borderColorV   = config.texBorderColorV;
        _textureOpts.dtype          = config.texPixelDataType;
        _textureOpts.textureUnit    = 0;

        std::shared_ptr< CTexture > _texturePtr;
        _texturePtr.reset( new CTexture( _textureDataPtr, _textureOpts ) );

        m_textures[config.name] = _texturePtr;

        /************************************************************************/

        // add the attachment to our framebuffer object
        glFramebufferTexture2D( GL_FRAMEBUFFER, 
                                toOpenGLEnum( config.attachment ), 
                                GL_TEXTURE_2D,
                                _texturePtr->openglId(),
                                0 );

        // sanity check: make sure framebuffer is correctly configured
        if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
            ENGINE_CORE_ERROR( "There seems to be an issue with a frame buffer" );

        // release our current framebuffer object
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }

    void CFrameBuffer::bind()
    {
        glBindFramebuffer( GL_FRAMEBUFFER, m_openglId );
    }

    void CFrameBuffer::unbind()
    {
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }


    std::shared_ptr< CTexture > CFrameBuffer::getTextureAttachment( const std::string& name )
    {
        if ( m_textures.find( name ) == m_textures.end() )
        {
            ENGINE_CORE_ERROR( "Tried to grab non-existent tex-attachment for id: {0}", name );
            return nullptr;
        }

        return m_textures[name];
    }

    CAttachmentConfig CFrameBuffer::getConfigAttachment( const std::string& name )
    {
        if ( m_configs.find( name ) == m_configs.end() )
        {
            ENGINE_CORE_ERROR( "Tried to grab non-existent config-attachment for id: {0}", name );
            return CAttachmentConfig();
        }

        return m_configs[name];
    }

}