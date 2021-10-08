
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

    CFrameBuffer::CFrameBuffer( int32 width, int32 height )
        : m_width( width ), m_height( height )
    {
        // create gl-texture resource (will configure it later)
        glGenFramebuffers( 1, &m_openglId );
    }

    CFrameBuffer::~CFrameBuffer()
    {
        glDeleteFramebuffers( 1, &m_openglId );

        for ( auto& pair : m_textures )
            delete pair.second;
        m_textures.clear();
        m_configs.clear();
    }

    void CFrameBuffer::addAttachment( const CAttachmentConfig& config )
    {
        m_configs[config.name] = config;

        // bind our current framebuffer object
        glBindFramebuffer( GL_FRAMEBUFFER, m_openglId );

        /* create dummy texture-data for texture attachment **********************/
        auto _textureData = std::make_unique<CTextureData>();
        _textureData->name              = config.name;
        _textureData->data              = NULL;
        _textureData->width             = m_width;
        _textureData->height            = m_height;
        _textureData->channels          = 0; // not used for our case
        _textureData->internalFormat    = config.texInternalFormat;
        _textureData->format            = config.texFormat;

        CTextureOptions _textureOpts;
        _textureOpts.filterMin      = eTextureFilter::NEAREST;
        _textureOpts.filterMag      = eTextureFilter::NEAREST;
        _textureOpts.wrapU          = config.texWrapU;
        _textureOpts.wrapV          = config.texWrapV;
        _textureOpts.borderColorU   = config.texBorderColorU;
        _textureOpts.borderColorV   = config.texBorderColorV;
        _textureOpts.dtype          = config.texPixelDataType;

        auto _texture = new CTexture( std::move( _textureData ), _textureOpts );

        /************************************************************************/

        // add the attachment to our framebuffer object
        glFramebufferTexture2D( GL_FRAMEBUFFER,
                                toOpenGLEnum( config.attachment ),
                                GL_TEXTURE_2D,
                                _texture->openglId(),
                                0 );

        // sanity check: make sure framebuffer is correctly configured (@TODO: might be an issue with only depth-attach)
        if ( glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE )
            ENGINE_CORE_ERROR( "CFrameBuffer::addAttachment >>> There seems to be an issue with a frame buffer" );

        // release our current framebuffer object
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );

        // keep ownership of the texture of this attachment
        m_textures[config.name] = _texture;
    }

    void CFrameBuffer::bind()
    {
        glBindFramebuffer( GL_FRAMEBUFFER, m_openglId );
    }

    void CFrameBuffer::unbind()
    {
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }

    void CFrameBuffer::resize( int32 width, int32 height )
    {
        m_width = width;
        m_height = height;
        // should resize textures to handle the required available space
        for ( auto& pair : m_textures )
            pair.second->resize( width, height );
    }

    std::unique_ptr< uint8[] > CFrameBuffer::read()
    {
        auto _colorAttachment = getTextureAttachment( "color_attachment" );
        auto _colorAttachmentConfig = getConfigAttachment( "color_attachment" );

        if ( !_colorAttachment )
        {
            // @firsttodo: make this more general (it's specific to this particular attachment)
            ENGINE_CORE_WARN( "CFrameBuffer::read >>> can't read from framebuffer without color attachment" );
            return nullptr;
        }

        auto _buffer = std::unique_ptr< engine::uint8[] >( new engine::uint8[3 * m_width * m_height] );

        glBindFramebuffer( GL_FRAMEBUFFER, m_openglId );
        glReadPixels( 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, _buffer.get() );
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );

        struct _Pixel { uint8 r; uint8 g; uint8 b; };
        auto _pixelBuffer = (_Pixel*) _buffer.get();
        for ( size_t i = 0; i < std::floor( m_height / 2 ); i++ )
        {
            for ( size_t j = 0; j < m_width; j++ )
            {
                auto _srcIndex = j + i * m_width;
                auto _dstIndex = j + ( m_height - i - 1 ) * m_width;
                auto _temp = _pixelBuffer[_srcIndex];
                _pixelBuffer[_srcIndex] = _pixelBuffer[_dstIndex];
                _pixelBuffer[_dstIndex] = _temp;
            }
        }

        return std::move( _buffer );
    }

    CTexture* CFrameBuffer::getTextureAttachment( const std::string& name )
    {
        if ( m_textures.find( name ) == m_textures.end() )
        {
            ENGINE_CORE_ERROR( "CFrameBuffer::getTextureAttachment >>> Tried to grab non-existent tex-attachment for id: {0}", name );
            return nullptr;
        }

        return m_textures[name];
    }

    CAttachmentConfig CFrameBuffer::getConfigAttachment( const std::string& name )
    {
        if ( m_configs.find( name ) == m_configs.end() )
        {
            ENGINE_CORE_ERROR( "CFrameBuffer::getConfigAttachment >>> Tried to grab non-existent config-attachment for id: {0}", name );
            return CAttachmentConfig();
        }

        return m_configs[name];
    }

    std::map< std::string, CTexture* > CFrameBuffer::textures() const
    {
        std::map< std::string, CTexture* > _texturesMap;

        for ( auto& pair : m_textures )
            _texturesMap[pair.first] = pair.second;

        return _texturesMap;
    }

    std::map< std::string, CAttachmentConfig > CFrameBuffer::configs() const
    {
        return m_configs;
    }

}
