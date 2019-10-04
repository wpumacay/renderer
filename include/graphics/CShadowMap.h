#pragma once

#include <camera/CICamera.h>
#include <lights/CILight.h>
#include <lights/CDirectionalLight.h>
#include <lights/CPointLight.h>
#include <lights/CSpotLight.h>
#include <core/CFrameBuffer.h>

namespace engine
{

    enum class eShadowRangeType
    {
        NONE = 0,
        AUTOFIX_CAMERA, // matrices are computed using the current camera frustum
        FIXED_USER,     // matrices are defined by the user for a given region of interest
    };

    std::string toString( const eShadowRangeType& rangeType );

    struct CShadowMapRangeConfig
    {
        eShadowRangeType    type;
        CVec3               worldUp;
        /* light casters used for the shadowmap */
        CPointLight*        pointLightPtr;
        CSpotLight*         spotLightPtr;
        CDirectionalLight*  dirLightPtr;
        /* configuration used for autofix-camera */
        CICamera* cameraPtr;
        float32   extraWidth;
        float32   extraHeight;
        float32   extraDepth;
        /* configuration used for fixed-by-user */
        CVec3     focusPoint;
        float32   clipSpaceWidth;
        float32   clipSpaceHeight;
        float32   clipSpaceDepth;

        CShadowMapRangeConfig()
        {
            type            = eShadowRangeType::NONE;
            worldUp         = { 0.0f, 1.0f, 0.0f };
            pointLightPtr   = nullptr;
            spotLightPtr    = nullptr;
            dirLightPtr     = nullptr;
            cameraPtr       = nullptr;
            extraWidth      = 1.0f;
            extraHeight     = 1.0f;
            extraDepth      = 1.0f;
            focusPoint      = { 0.0f, 0.0f, 0.0f };
            clipSpaceWidth  = 20.0f;
            clipSpaceHeight = 20.0f;
            clipSpaceDepth  = 20.0f;
        }
    };

    struct CComparatorDotDirection
    {
        engine::CVec3 direction;
    
        bool operator() ( CVec3 v1, CVec3 v2 )
        {
            auto _dot1 = CVec3::dot( direction, v1 );
            auto _dot2 = CVec3::dot( direction, v2 );
    
            return _dot1 < _dot2;
        }
    };

    class CShadowMap
    {

    public :

        CShadowMap( int32 width, int32 height );
        ~CShadowMap();

        void bind();
        void unbind();

        void setup( const CShadowMapRangeConfig& config );

        CFrameBuffer* frameBuffer() const { return m_frameBuffer.get(); };

        int32 width() const { return m_shadowMapWidth; }
        int32 height() const { return m_shadowMapHeight; }

        CMat4 lightSpaceMatView() const { return m_lightSpaceMatView; }
        CMat4 lightSpaceMatProj() const { return m_lightSpaceMatProj; }

    private :

        void _setupDirectionalLight( const CShadowMapRangeConfig& config, CDirectionalLight* dirLightPtr );
        void _setupPointLight( const CShadowMapRangeConfig& config, CPointLight* pointLightPtr );
        void _setupSpotLight( const CShadowMapRangeConfig& config, CSpotLight* spotLightPtr );

        /* FBO object (with only depth attachment) */
        std::unique_ptr< CFrameBuffer > m_frameBuffer;

        /* view-matrix from the point of view of the light (where to look when projecting to the depth-matp) */
        CMat4 m_lightSpaceMatView;

        /* proj-matrix from the point of view of the light (what to project into depth-map) */
        CMat4 m_lightSpaceMatProj;

        /* shadowmap dimensions (used to set the viewport) */
        int32 m_shadowMapWidth;
        int32 m_shadowMapHeight;

        /* viewport configuration to be restored after using the shadowmap */
        int32 m_previousViewportX;
        int32 m_previousViewportY;
        int32 m_previousViewportWidth;
        int32 m_previousViewportHeight;

        /* frustum points in clip-space (used for frustum-points in world-space computation) */
        std::array< CVec3, 8 > m_frustumPointsClipSpace;

        /* comparator used to sort points according to the dot-product to some other vector */
        CComparatorDotDirection m_comparatorDotDirection;
    };

}