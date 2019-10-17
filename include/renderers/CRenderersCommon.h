#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>
#include <camera/CICamera.h>
#include <core/CFrameBuffer.h>
#include <graphics/CFog.h>
#include <graphics/CSkybox.h>
#include <graphics/CShadowMap.h>
#include <graphics/CIRenderable.h>
#include <graphics/CMesh.h>
#include <graphics/CModel.h>
#include <graphics/CScene.h>
#include <lights/CILight.h>
#include <lights/CDirectionalLight.h>
#include <lights/CPointLight.h>
#include <lights/CSpotLight.h>
#include <shaders/CShaderManager.h>
#include <shaders/CShader.h>

namespace engine
{

    enum class eRenderMode
    {
        NONE = 0,
        NORMAL,
        DEPTH_ONLY,
        SEMANTIC_ONLY,
        NO_SUBMIT
    };

    std::string toString( const eRenderMode& mode );

    enum class eCullingGeom
    {
        NONE = 0,
        BOUNDING_BOX,
        BOUNDING_SPHERE
    };

    std::string toString( const eCullingGeom& geom );

    struct CRenderOptions
    {
        eRenderMode             mode;
        bool                    useFrustumCulling;
        eCullingGeom            cullingGeom;
        bool                    useFaceCulling;
        bool                    useShadowMapping;
        bool                    redrawShadowMap;
        int                     pcfCount;
        bool                    useBlending;
        bool                    useFog;
        bool                    useSkybox;
        float32                 skyboxFogLowerLimit;
        float32                 skyboxFogUpperLimit;
        int32                   viewportWidth;
        int32                   viewportHeight;
        CShadowMapRangeConfig   shadowMapRangeConfig;
        CICamera*               cameraPtr;
        CILight*                lightPtr;
        CShadowMap*             shadowMapPtr;
        CFrameBuffer*           renderTargetPtr;
        CFog*                   fogPtr;
        CSkybox*                skyboxPtr;
        /* depth_only-mode parameters */
        float32                 depthViewZmin;
        float32                 depthViewZmax;
        CVec3                   depthViewZminColor;
        CVec3                   depthViewZmaxColor;
        /* semantic_only-mode parameters */
        std::unordered_map< int32, CVec3 > semanticViewIdMap;

        CRenderOptions()
        {
            mode                    = eRenderMode::NORMAL;
            useFrustumCulling       = false;
            cullingGeom             = eCullingGeom::BOUNDING_SPHERE;
            useFaceCulling          = false;
            useShadowMapping        = false;
            redrawShadowMap         = true;
            pcfCount                = 1;
            useBlending             = false;
            useFog                  = false;
            useSkybox               = false;
            skyboxFogLowerLimit     = 0.0f;
            skyboxFogUpperLimit     = 0.25f;
            viewportWidth           = -1;// default (-1) means use current viewport
            viewportHeight          = -1;// default (-1) means use current viewport
            shadowMapRangeConfig    = CShadowMapRangeConfig();
            cameraPtr               = nullptr;
            lightPtr                = nullptr;
            shadowMapPtr            = nullptr;
            renderTargetPtr         = nullptr;
            fogPtr                  = nullptr;
            skyboxPtr               = nullptr;

            depthViewZmin       = 0.0f;
            depthViewZmax       = 6.0f;
            depthViewZminColor  = { 1.0f, 1.0f, 1.0f };
            depthViewZmaxColor  = { 0.0f, 0.0f, 0.0f };

            semanticViewIdMap = std::unordered_map< int32, CVec3 >();
        }
    };

    std::string toString( const CRenderOptions& renderOptions );

}