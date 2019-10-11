
#include <renderers/CMainRenderer.h>


namespace engine
{

    std::string toString( const eRenderMode& mode )
    {
        if ( mode == eRenderMode::NORMAL ) return "normal";
        if ( mode == eRenderMode::DEPTH_ONLY ) return "depth_only";
        if ( mode == eRenderMode::SEMANTIC_ONLY ) return "semantic_only";

        ENGINE_CORE_ASSERT( false, "Invalid eRenderMode enum given" );

        return "undefined";
    }

    std::string toString( const eCullingGeom& geom )
    {
        if ( geom == eCullingGeom::BOUNDING_BOX ) return "bounding_box";
        if ( geom == eCullingGeom::BOUNDING_SPHERE ) return "bounding_sphere";

        ENGINE_CORE_ASSERT( false, "Invalid eCullingGeom enum given" );

        return "undefined";
    }

    std::string toString( const CRenderOptions& renderOptions )
    {
        std::string _strRep;

        _strRep += "render-mode             : " + engine::toString( renderOptions.mode ) + "\n\r";
        _strRep += "render-viewport-width   : " + std::to_string( renderOptions.viewportWidth ) + "\n\r";
        _strRep += "render-viewport-height  : " + std::to_string( renderOptions.viewportHeight ) + "\n\r";

        return _strRep;
    }

    CMainRenderer::CMainRenderer()
    {
        m_shadowMap = std::unique_ptr< CShadowMap >( new CShadowMap( RENDERER_SHADOWMAP_WIDTH,
                                                                     RENDERER_SHADOWMAP_HEIGHT ) );
    }

    CMainRenderer::~CMainRenderer()
    {
        m_shadowMap = nullptr;
    }

    void CMainRenderer::render( CScene* scenePtr, 
                                CICamera* cameraPtr, 
                                CFrameBuffer* targetPtr,
                                const CRenderOptions& renderOptions )
    {
        // (0): grab all renderables and initialize status string
        auto _renderablesAll = scenePtr->renderables();
        m_status = "renderables         : " + std::to_string( _renderablesAll.size() ) + "\n\r";

        // (1): grab all renderables and keep only visible ones
        auto _renderablesVisible = std::vector< CIRenderable* >();
        for ( auto _renderable : _renderablesAll )
            if ( _renderable->visible() )
                _renderablesVisible.push_back( _renderable );
        m_status += "renderablesVisible : " + std::to_string( _renderablesVisible.size() ) + "\n\r";

        // (2): frustum culling (if enabled)
        CFrustum _frustum( cameraPtr->matProj() * cameraPtr->matView() );
        auto _renderablesInView = std::vector< CIRenderable* >();
        if ( renderOptions.useFrustumCulling )
            _collectRenderablesInView( _frustum, renderOptions.cullingGeom, _renderablesVisible, _renderablesInView );
        else
            _renderablesInView = _renderablesVisible;
        m_status += "renderablesInView  : " + std::to_string( _renderablesInView.size() ) + "\n\r";

////         // (3): group by renderable types (to pass to specific renderers)
////         auto _rendGizmos = std::vector< CGizmo* >();
////         auto _rendMeshes = std::vector< CMesh* >();
////         auto _rendModels = std::vector< CModel* >();
////         _collectRenderablesByType<CGizmo>( _renderablesInView, _rendGizmos );
////         _collectRenderablesByType<CMesh>( _renderablesInView, _rendMeshes );
////         _collectRenderablesByType<CModel>( _renderablesInView, _rendModels );
//// 
////         // (4) submit to specific renderers for them to do extra preparations
////         m_rendererGizmos->submit( _rendGizmos, cameraPtr, renderOptions );
////         m_rendererMeshes->submit( _rendMeshes, cameraPtr, renderOptions );
////         m_rendererModels->submit( _rendModels, cameraPtr, renderOptions );
//// 
////         // (5) start making the actual rendering process (use forward rendering for now)
//// 
////         /* (5.1) render pass for shadow mapping (if enabled) */
////         if ( renderOptions.useShadowMapping )
////         {
////             m_shadowMap->bind();
////             m_rendererMeshes->renderToShadowMap( m_shadowMap.get(), renderOptions );
////             m_rendererModels->renderToShadowMap( m_shadowMap.get(), renderOptions );
////             m_shadowMap->unbind();
////         }
//// 
////         /* (5.2) setup render target if given*/
////         if ( renderOptions.renderTargetPtr )
////             renderOptions.renderTargetPtr->bind();
//// 
////         /* (5.3) render pass for the scene itself */
////         if ( renderOptions.useShadowMapping )
////         {
////             m_rendererMeshes->renderWithShadowMap( cameraPtr, m_shadowMap.get(), renderOptions );
////             m_rendererModels->renderWithShadowMap( cameraPtr, m_shadowMap.get(), renderOptions );
////         }
////         else
////         {
////             m_rendererMeshes->renderWithoutShadowMap( cameraPtr, renderOptions );
////             m_rendererModels->renderWithoutShadowMap( cameraPtr, renderOptions );
////         }
//// 
////         /* (5.4) render gizmos as well if they're required */
////         if ( renderOptions.renderGizmos )
////             m_rendererGizmos->render( renderOptions );
//// 
////         /* (5.5) release custom render target in case used */
////         if ( renderOptions.renderTargetPtr )
////             renderOptions.renderTargetPtr->unbind();
    }

    void CMainRenderer::_collectRenderablesInView( const CFrustum& frustum,
                                                   const eCullingGeom& cullGeometryToUse,
                                                   const std::vector< CIRenderable* >& renderablesToCheck,
                                                   std::vector< CIRenderable* >& renderablesInView )
    {
        if ( cullGeometryToUse == eCullingGeom::BOUNDING_BOX )
            _collectRenderablesInView_bbox( frustum, renderablesToCheck, renderablesInView );
        else if ( cullGeometryToUse == eCullingGeom::BOUNDING_SPHERE )
            _collectRenderablesInView_bsphere( frustum, renderablesToCheck, renderablesInView );
        else
            ENGINE_CORE_WARN( "The given culling method is not supported" );
    }

    void CMainRenderer::_collectRenderablesInView_bbox( const CFrustum& frustum,
                                                        const std::vector< CIRenderable* >& renderablesToCheck,
                                                        std::vector< CIRenderable* >& renderablesInView )
    {
        for ( auto renderablePtr : renderablesToCheck )
            if ( !engine::certainlyOutsideFrustum( frustum, renderablePtr->bbox() ) )
                renderablesInView.push_back( renderablePtr );
    }

    void CMainRenderer::_collectRenderablesInView_bsphere( const CFrustum& frustum,
                                                           const std::vector< CIRenderable* >& renderablesToCheck,
                                                           std::vector< CIRenderable* >& renderablesInView )
    {
        for ( auto renderablePtr : renderablesToCheck )
            if ( !engine::certainlyOutsideFrustum( frustum, renderablePtr->bsphere() ) )
                renderablesInView.push_back( renderablePtr );
    }

}