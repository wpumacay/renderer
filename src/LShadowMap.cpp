
#include "LShadowMap.h"



namespace engine
{


    LShadowMap::LShadowMap( int width, int height )
    {
        m_width = width;
        m_height = height;

        m_prevViewWidth  = -1;
        m_prevViewHeight = -1;

        m_shadowBuffer = new LFrameBuffer( GL_DEPTH_COMPONENT, GL_FLOAT, 
                                           GL_DEPTH_ATTACHMENT,
                                           m_width, m_height );
        m_shadowBuffer->bind();

        glDrawBuffer( GL_NONE );
        glReadBuffer( GL_NONE );

        glBindTexture( GL_TEXTURE_2D, m_shadowBuffer->getTex() );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
        float _borderValues[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, _borderValues );

        glBindTexture( GL_TEXTURE_2D, 0 );

        m_shadowBuffer->unbind();
    }


    LShadowMap::~LShadowMap()
    {
        if ( m_shadowBuffer != NULL )
        {
            delete m_shadowBuffer;
            m_shadowBuffer = NULL;
        }
    }

    void LShadowMap::setupLightDirectional( LLightDirectional* pLight )
    {
        float _zNear = 1.0f;
        float _zFar  = 20.0f;

        float _width  = 20.0f;
        float _height = 20.0f;

        m_projMat = glm::ortho<float>( -0.5f * _width, 0.5f * _width, -0.5 * _height, 0.5 * _height, _zNear, _zFar );

        auto _vpos = pLight->getVirtualPosition();
        glm::vec3 _virtualPosition = glm::vec3( _vpos.x, _vpos.y, _vpos.z );
        glm::vec3 _virtualTarget = _virtualPosition + glm::vec3( pLight->direction.x, pLight->direction.y, pLight->direction.z );

        m_viewMat = glm::lookAt( _virtualPosition, _virtualTarget, glm::vec3( 0.0f, 1.0f, 0.0f ) );
        m_virtualLightPosition = LVec3( _virtualPosition.x, _virtualPosition.y, _virtualPosition.z );
    }

    void LShadowMap::bind()
    {
        GLint _currentViewPort[4];
        glGetIntegerv( GL_VIEWPORT, _currentViewPort );

        m_prevViewWidth  = _currentViewPort[2];
        m_prevViewHeight = _currentViewPort[3];
        

        glViewport( 0, 0, m_width, m_height );
        m_shadowBuffer->bind();
        glClear( GL_DEPTH_BUFFER_BIT );
    }

    void LShadowMap::unbind()
    {
        m_shadowBuffer->unbind();

        glViewport( 0, 0, m_prevViewWidth, m_prevViewHeight );
    }

}

