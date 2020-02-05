
#include <graphics/CShadowMap.h>

namespace engine
{

    std::string toString( const eShadowRangeType& rangeType )
    {
        if ( rangeType == eShadowRangeType::AUTOFIX_CAMERA ) return "autofix_camera";
        if ( rangeType == eShadowRangeType::FIXED_USER ) return "fixed_user";

        ENGINE_CORE_ASSERT( false, "Invalid eShadowRangeType enum given" );

        return "undefined";
    }

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
        _fbDepthConfig.texInternalFormat    = eTextureFormat::DEPTH;
        _fbDepthConfig.texFormat            = eTextureFormat::DEPTH;
        _fbDepthConfig.texPixelDataType     = ePixelDataType::FLOAT_32;
        _fbDepthConfig.texWrapU             = eTextureWrap::CLAMP_TO_BORDER;
        _fbDepthConfig.texWrapV             = eTextureWrap::CLAMP_TO_BORDER;
        _fbDepthConfig.texBorderColorU      = { 1.0f, 1.0f, 1.0f, 1.0f };
        _fbDepthConfig.texBorderColorV      = { 1.0f, 1.0f, 1.0f, 1.0f };

        /* create the framebuffer with an attachment given by the config. above */
        m_frameBuffer = std::make_unique< CFrameBuffer >( m_shadowMapWidth, m_shadowMapHeight );
        m_frameBuffer->addAttachment( _fbDepthConfig );

        /* tell opengl we don't need a color attachment */
        m_frameBuffer->bind();
        glDrawBuffer( GL_NONE );
        glReadBuffer( GL_NONE );
        m_frameBuffer->unbind();

        /* define the points in clip-space of the vertices of the view-frustum */
        /*      near plane      */
        m_frustumPointsClipSpace[0] = { -1.0f, -1.0f, -1.0f };
        m_frustumPointsClipSpace[1] = {  1.0f, -1.0f, -1.0f };
        m_frustumPointsClipSpace[2] = {  1.0f,  1.0f, -1.0f };
        m_frustumPointsClipSpace[3] = { -1.0f,  1.0f, -1.0f };
        /*      far plane       */
        m_frustumPointsClipSpace[4] = { -1.0f, -1.0f, 1.0f };
        m_frustumPointsClipSpace[5] = {  1.0f, -1.0f, 1.0f };
        m_frustumPointsClipSpace[6] = {  1.0f,  1.0f, 1.0f };
        m_frustumPointsClipSpace[7] = { -1.0f,  1.0f, 1.0f };
    }

    CShadowMap::~CShadowMap()
    {
        // nothing to release explicitly
    }

    void CShadowMap::setup( const CShadowMapRangeConfig& config )
    {
        // sanity check: if using autofix-camera, make sure we are given a camera
        if ( config.type == eShadowRangeType::AUTOFIX_CAMERA && !config.cameraPtr )
        {
            ENGINE_CORE_ERROR( "Must provide a valid camera for AUTOFIX_CAMERA mode" );
            return;
        }

        if ( config.dirLightPtr )
            _setupDirectionalLight( config, config.dirLightPtr );
        else if ( config.pointLightPtr )
            _setupPointLight( config, config.pointLightPtr );
        else if ( config.spotLightPtr )
            _setupSpotLight( config, config.spotLightPtr );
        else
            ENGINE_CORE_ERROR( "Didn't provide a light to configure shadow-map" );
    }

    void CShadowMap::_setupDirectionalLight( const CShadowMapRangeConfig& config, CDirectionalLight* dirLightPtr )
    {
        if ( config.type == eShadowRangeType::AUTOFIX_CAMERA )
        {
            // Configure the clipping volume for shadow-mapping such that it 
            // is an OOBB to the view camera's view frustum

            /* compute the vertices of the view frustum */
            auto _invViewProjMatrix = tinymath::inverse( config.cameraPtr->matProj() * config.cameraPtr->matView() );

            std::vector< CVec3 > _frustumPointsInWorld;
            for ( size_t q = 0; q < m_frustumPointsClipSpace.size(); q++ )
            {
                CVec4 _frustumPointNormalized = _invViewProjMatrix * CVec4( m_frustumPointsClipSpace[q], 1.0f );
                CVec3 _frustumPoint = CVec3( _frustumPointNormalized.x() / _frustumPointNormalized.w(),
                                             _frustumPointNormalized.y() / _frustumPointNormalized.w(),
                                             _frustumPointNormalized.z() / _frustumPointNormalized.w() );

                _frustumPointsInWorld.push_back( _frustumPoint );
            }

            /* construct a frame using the direction vector as front */
            CVec3 _fvec, _rvec, _uvec;

            if ( dirLightPtr->direction == config.worldUp )
            {
                _fvec = config.worldUp;
                _rvec = CVec3( config.worldUp.z(), config.worldUp.x(), config.worldUp.y() );
                _uvec = CVec3( config.worldUp.y(), config.worldUp.z(), config.worldUp.x() );
            }
            else if ( ( dirLightPtr->direction + config.worldUp ) == CVec3( 0.0f, 0.0f, 0.0f ) )
            {
                _fvec = -config.worldUp;
                _rvec = CVec3( config.worldUp.z(), config.worldUp.x(), config.worldUp.y() );
                _uvec = CVec3( config.worldUp.y(), config.worldUp.z(), config.worldUp.x() );
            }
            else
            {
                _fvec = dirLightPtr->direction;
                _rvec = tinymath::cross( config.worldUp, _fvec );
                _uvec = tinymath::cross( _fvec, _rvec );
            }

            _fvec.normalize();
            _rvec.normalize();
            _uvec.normalize();

            /* sort over f-vector */
            auto _fPoints3d = _frustumPointsInWorld; // create a copy
            {
                m_comparatorDotDirection.direction = _fvec;
                std::sort( _fPoints3d.begin(), _fPoints3d.end(), m_comparatorDotDirection );
            }
            /* sort over r-vector */
            auto _rPoints3d = _frustumPointsInWorld; // create a copy
            {
                m_comparatorDotDirection.direction = _rvec;
                std::sort( _rPoints3d.begin(), _rPoints3d.end(), m_comparatorDotDirection );
            }
            /* sort over u-vector */
            auto _uPoints3d = _frustumPointsInWorld; // create a copy
            {
                m_comparatorDotDirection.direction = _uvec;
                std::sort( _uPoints3d.begin(), _uPoints3d.end(), m_comparatorDotDirection );
            }

            /* compute the dimensions of the clipping volume */
            float32 _df = std::abs( ( _fPoints3d.back() - _fPoints3d.front() ).dot( _fvec ) );
            float32 _dr = std::abs( ( _rPoints3d.back() - _rPoints3d.front() ).dot( _rvec ) );
            float32 _du = std::abs( ( _uPoints3d.back() - _uPoints3d.front() ).dot( _uvec ) );

            /* compute the center of the clipping volume */
            auto _center = ( 0.5f * ( _fPoints3d.front() + _fPoints3d.back() ) ).dot( _fvec ) * _fvec +
                           ( 0.5f * ( _rPoints3d.front() + _rPoints3d.back() ) ).dot( _rvec ) * _rvec +
                           ( 0.5f * ( _uPoints3d.front() + _uPoints3d.back() ) ).dot( _uvec ) * _uvec;

            /* construct view and proj matrices in light space */
            auto _position = _center - ( 0.5f * _df ) * _fvec;
            auto _target = _position + dirLightPtr->direction;
            m_lightSpaceMatView = engine::lookAt( _position, _target, config.worldUp );
            m_lightSpaceMatProj = engine::ortho( _dr + config.extraWidth, 
                                                 _du + config.extraHeight, 
                                                 -0.5f * config.extraDepth, 
                                                 _df + 0.5f * config.extraDepth );
        }
        else if ( config.type == eShadowRangeType::FIXED_USER )
        {
            // Configure the clipping volume for shadow mapping according to the
            // configuration parameters given by the user (focus on an area).
            // Setup adapted from DeepMimic shadow-pass setup to obtain similar results (his graphics look nice as well :D):
            //  https://github.com/xbpeng/DeepMimic/blob/fbd77f66f7e0ee9d7cd2c863636f296f3af45e04/DeepMimicCore/scenes/DrawScene.cpp#L560

            /*         \
            *          /\
            *         /  \
            *        /    \
            *       /     (*)  position of clip-volume (znear)
            * (depth)     /
            *     /      /
            *    /      /
            *   /     (*)  focus-point
            * \/     /
            *  \    /
            *   \  /
            *    (*) (zfar)
            */

            /* compute the position of the clipping volume from the point we want to focus */
            auto _position = config.focusPoint - 0.5f * config.clipSpaceDepth * dirLightPtr->direction;
            m_lightSpaceMatView = engine::lookAt( _position, config.focusPoint, config.worldUp );
            m_lightSpaceMatProj = engine::ortho( config.clipSpaceWidth, 
                                                 config.clipSpaceHeight, 
                                                 0.0f, config.clipSpaceDepth );
        }
        else
            ENGINE_CORE_ERROR( "Invalid range-type parameter given while setting up shadowmap" );
    }

    void CShadowMap::_setupPointLight( const CShadowMapRangeConfig& config, CPointLight* pointLightPtr )
    {
        if ( config.type == eShadowRangeType::AUTOFIX_CAMERA )
        {
            ENGINE_CORE_WARN( "Point-lights don't support autofix functionality yet :(. Use a directional light instead, or use FIXED_USER" );
        }
        else if ( config.type == eShadowRangeType::FIXED_USER )
        {
            m_lightSpaceMatView = engine::lookAt( pointLightPtr->position, config.focusPoint, config.worldUp );
            m_lightSpaceMatProj = engine::perspective( config.clipSpaceFov, 
                                                       ((float)m_shadowMapWidth) / m_shadowMapHeight, 
                                                       config.clipSpaceZNear, 
                                                       config.clipSpaceZFar );
        }
    }

    void CShadowMap::_setupSpotLight( const CShadowMapRangeConfig& config, CSpotLight* spotLightPtr )
    {
        if ( config.type == eShadowRangeType::AUTOFIX_CAMERA )
        {
            ENGINE_CORE_WARN( "Spot-lights don't support autofix functionality yet :(. Use a directional light instead, or use FIXED_USER" );
        }
        else if ( config.type == eShadowRangeType::FIXED_USER )
        {
            m_lightSpaceMatView = engine::lookAt( spotLightPtr->position, 
                                                  spotLightPtr->position + spotLightPtr->direction, 
                                                  config.worldUp );
            m_lightSpaceMatProj = engine::perspective( config.clipSpaceFov, 
                                                       ((float)m_shadowMapWidth) / m_shadowMapHeight, 
                                                       config.clipSpaceZNear, 
                                                       config.clipSpaceZFar );
        }
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