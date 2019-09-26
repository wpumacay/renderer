
#include "LShadowMap.h"

namespace engine
{


    LShadowMap::LShadowMap( int width, int height )
    {
        m_width = width;
        m_height = height;

        m_prevViewWidth  = -1;
        m_prevViewHeight = -1;

        CAttachmentConfig _fbDepthConfig;
        _fbDepthConfig.name                 = "shadow_depth_attachment";
        _fbDepthConfig.attachment           = eFboAttachment::DEPTH;
        _fbDepthConfig.width                = m_width;
        _fbDepthConfig.height               = m_height;
        _fbDepthConfig.texInternalFormat    = eTextureFormat::DEPTH;
        _fbDepthConfig.texFormat            = eTextureFormat::DEPTH;
        _fbDepthConfig.texPixelDataType     = ePixelDataType::FLOAT_32;
        _fbDepthConfig.texWrapU             = eTextureWrap::CLAMP_TO_BORDER;
        _fbDepthConfig.texWrapV             = eTextureWrap::CLAMP_TO_BORDER;
        _fbDepthConfig.texBorderColorU      = { 1.0f, 1.0f, 1.0f, 1.0f };
        _fbDepthConfig.texBorderColorV      = { 1.0f, 1.0f, 1.0f, 1.0f };

        m_framebuffer = new CFrameBuffer();
        m_framebuffer->addAttachment( _fbDepthConfig );

        m_framebuffer->bind();
        glDrawBuffer( GL_NONE );
        glReadBuffer( GL_NONE );
        m_framebuffer->unbind();
    }


    LShadowMap::~LShadowMap()
    {
        if ( m_framebuffer != NULL )
        {
            delete m_framebuffer;
            m_framebuffer = NULL;
        }
    }

    void LShadowMap::setupLightDirectional( CDirectionalLight* pLight )
    {
        float _zNear = 1.0f;
        float _zFar  = 20.0f;

        float _width  = 20.0f;
        float _height = 20.0f;

//        m_projMat = CMat4::ortho( _width, _height, _zNear, _zFar );
//
//        auto _vpos = pLight->getVirtualPosition();
//        auto _vtarget = _vpos + pLight->direction;
//
//        m_viewMat = CMat4::lookAt( _vpos, _vtarget, CVec3( 0.0f, 1.0f, 0.0f ) );
//        m_virtualLightPosition = _vpos;
    }

    void LShadowMap::bind()
    {
        GLint _currentViewPort[4];
        glGetIntegerv( GL_VIEWPORT, _currentViewPort );

        m_prevViewWidth  = _currentViewPort[2];
        m_prevViewHeight = _currentViewPort[3];
        

        glViewport( 0, 0, m_width, m_height );
        m_framebuffer->bind();
        glClear( GL_DEPTH_BUFFER_BIT );
    }

    void LShadowMap::unbind()
    {
        m_framebuffer->unbind();

        glViewport( 0, 0, m_prevViewWidth, m_prevViewHeight );
    }

    GLuint LShadowMap::getDepthTexture() const
    {
        return m_framebuffer->getTextureAttachment( "shadow_depth_attachment" )->openglId();
    }

}

