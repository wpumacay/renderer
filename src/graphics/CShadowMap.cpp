
#include <graphics/CShadowMap.h>

namespace engine
{

    CShadowMap::CShadowMap( int32 width, int32 height )
    {
        m_shadowMapWidth = width;
        m_shadowMapHeight = height;

        m_previousViewportX = -1;
        m_previousViewportY = -1;
        m_previousViewportWidth  = -1;
        m_previousViewportHeight = -1;

        /* configure the framebuffer used to render the depth-map */
        CAttachmentConfig _fbDepthConfig;
        _fbDepthConfig.name                 = "shadow_depth_attachment";
        _fbDepthConfig.attachment           = eFboAttachment::DEPTH;
        _fbDepthConfig.width                = m_shadowMapWidth;
        _fbDepthConfig.height               = m_shadowMapHeight;
        _fbDepthConfig.texInternalFormat    = eTextureFormat::DEPTH;
        _fbDepthConfig.texFormat            = eTextureFormat::DEPTH;
        _fbDepthConfig.texPixelDataType     = ePixelDataType::FLOAT_32;
        _fbDepthConfig.texWrapU             = eTextureWrap::CLAMP_TO_BORDER;
        _fbDepthConfig.texWrapV             = eTextureWrap::CLAMP_TO_BORDER;
        _fbDepthConfig.texBorderColorU      = { 1.0f, 1.0f, 1.0f, 1.0f };
        _fbDepthConfig.texBorderColorV      = { 1.0f, 1.0f, 1.0f, 1.0f };

        /* create the framebuffer with an attachment given by the config. above */
        m_frameBuffer = std::unique_ptr< CFrameBuffer >( new CFrameBuffer() );
        m_frameBuffer->addAttachment( _fbDepthConfig );

        /* tell opengl we don't need a color attachment */
        m_frameBuffer->bind();
        glDrawBuffer( GL_NONE );
        glReadBuffer( GL_NONE );
        m_frameBuffer->unbind();
    }

    CShadowMap::~CShadowMap()
    {
        // nothing to release explicitly
    }

    void CShadowMap::setupDirectionalLight( CDirectionalLight* dirLightPtr )
    {

    }

    void CShadowMap::setupPointLight( CPointLight* pointLightPtr )
    {

    }

    void CShadowMap::setupSpotLight( CSpotLight* spotLightPtr )
    {

    }

    void CShadowMap::bind()
    {
        /* grab the previous viewport configuration (@TODO: might not be necessary if renderer handles it) */
        int32 _currentViewport[4];
        glGetIntegerv( GL_VIEWPORT, _currentViewport );
        m_previousViewportX = _currentViewport[0];
        m_previousViewportY = _currentViewport[1];
        m_previousViewportWidth  = _currentViewport[2];
        m_previousViewportHeight = _currentViewport[3];

        /* configure the viewport with the resolution of our depth-map */
        glViewport( 0, 0, m_shadowMapWidth, m_shadowMapHeight );

        /* bind the framebuffer for the depth-map and clear it before using it */
        m_frameBuffer->bind();
        glClear( GL_DEPTH_BUFFER_BIT );
    }

    void CShadowMap::unbind()
    {
        /* unbind the framebuffer (go back to the default gl-framebuffer) */
        m_frameBuffer->unbind();

        /* restore the viewport configuration previous to using the shadowmap */
        glViewport( m_previousViewportX, m_previousViewportY, m_previousViewportWidth, m_previousViewportHeight );
    }

}