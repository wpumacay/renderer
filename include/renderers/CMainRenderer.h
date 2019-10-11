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

#include <shaders/CShaderManager.h>
#include <shaders/CShader.h>

namespace engine
{

    const int32 RENDERER_SHADOWMAP_WIDTH    = 4096;
    const int32 RENDERER_SHADOWMAP_HEIGHT   = 4096;

    enum class eRenderMode
    {
        NONE = 0,
        NORMAL,
        DEPTH_ONLY,
        SEMANTIC_ONLY
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
        bool                    renderGizmos;
        int32                   viewportWidth;
        int32                   viewportHeight;
        CShadowMapRangeConfig   shadowMapRangeConfig;

        CRenderOptions()
        {
            mode                    = eRenderMode::NORMAL;
            useFrustumCulling       = false;
            cullingGeom             = eCullingGeom::BOUNDING_SPHERE;
            useFaceCulling          = false;
            useShadowMapping        = false;
            renderGizmos            = true;
            viewportWidth           = -1;// default (-1) means use current viewport
            viewportHeight          = -1;// default (-1) means use current viewport
            shadowMapRangeConfig    = CShadowMapRangeConfig();
        }
    };

    std::string toString( const CRenderOptions& renderOptions );

    class CMainRenderer
    {

    public :

        CMainRenderer();
        ~CMainRenderer();

        void render( CScene* scenePtr, 
                     CICamera* cameraPtr, 
                     CFrameBuffer* targetPtr,
                     const CRenderOptions& renderOptions = CRenderOptions() );

        std::string status() const { return m_status; }

    private :

        void _collectRenderablesInView( const CFrustum& frustum,
                                        const eCullingGeom& cullGeometryToUse,
                                        const std::vector< CIRenderable* >& renderablesToCheck,
                                        std::vector< CIRenderable* >& renderablesInView );

        void _collectRenderablesInView_bbox( const CFrustum& frustum,
                                             const std::vector< CIRenderable* >& renderablesToCheck,
                                             std::vector< CIRenderable* >& renderablesInView );

        void _collectRenderablesInView_bsphere( const CFrustum& frustum,
                                                const std::vector< CIRenderable* >& renderablesToCheck,
                                                std::vector< CIRenderable* >& renderablesInView );

        template< class T >
        void _collectRenderablesByType( const std::vector< CIRenderable* >& renderables,
                                        std::vector< T* >& renderablesOfGivenType )
        {
            for ( auto _renderable : renderables )
                if ( _renderable->type() == T::GetStaticType() )
                    renderablesOfGivenType.push_back( dynamic_cast<T*>( _renderable ) );
        }

    private :

        std::string m_status;
        std::unique_ptr< CShadowMap > m_shadowMap;

    };

}