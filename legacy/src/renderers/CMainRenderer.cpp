
#include <renderers/CMainRenderer.h>

namespace engine
{

    CMainRenderer::CMainRenderer()
    {
        m_shadowMap = std::unique_ptr< CShadowMap >( new CShadowMap( RENDERER_SHADOWMAP_WIDTH,
                                                                     RENDERER_SHADOWMAP_HEIGHT ) );

        m_rendererMeshes = std::unique_ptr< CMeshRenderer >( new CMeshRenderer() );
        m_rendererSkybox = std::unique_ptr< CSkyboxRenderer >( new CSkyboxRenderer() );
    }

    CMainRenderer::~CMainRenderer()
    {
        m_shadowMap = nullptr;
        m_rendererMeshes = nullptr;
        m_rendererSkybox = nullptr;
    }

    void CMainRenderer::begin( const CRenderOptions& renderOptions )
    {
        ENGINE_PROFILE_FUNCTION_IN_SESSION( "sess_core_render" );
        // keep a copy for later usage
        m_renderOptions = renderOptions;
        // assume we have valid info, and check if options given are correct
        m_hasValidInfo = true;

        /* do some error handling in case some options/resources are not correctly specified */
        if ( m_renderOptions.useShadowMapping && !m_renderOptions.shadowMapPtr )
        {
            ENGINE_CORE_WARN( "Renderer was setup to use shadow mapping, but no shadow-map provided" );
            m_renderOptions.useShadowMapping = false;
        }
        if ( m_renderOptions.useFog && !m_renderOptions.fogPtr )
        {
            ENGINE_CORE_WARN( "Renderer was setup to use fog, but no fog-struct provided" );
            m_renderOptions.useFog = false;
        }
        if ( m_renderOptions.useSkybox && !m_renderOptions.skyboxPtr )
        {
            ENGINE_CORE_WARN( "Renderer was setup to use a skybox, but no skybox-struct provided" );
            m_renderOptions.useSkybox = false;
        }

        /* check these options and invalidate this render request if not enough valid information is given */
        if ( !m_renderOptions.cameraPtr )
        {
            m_hasValidInfo = false;
            return;
        }
        if ( m_renderOptions.mode == eRenderMode::NORMAL && !m_renderOptions.lightPtr )
        {
            m_hasValidInfo = false;
            return;
        }

        // prepare shadowmap (configure its light-space matrices)
        if ( m_renderOptions.useShadowMapping )
            m_renderOptions.shadowMapPtr->setup( m_renderOptions.shadowMapRangeConfig );

        // prepare the mesh and skybox renderers
        m_rendererMeshes->begin( m_renderOptions );
        if ( m_renderOptions.useSkybox )
            m_rendererSkybox->begin( m_renderOptions );
    }

    void CMainRenderer::submit( const std::vector< CIRenderable* >& renderables )
    {
        ENGINE_PROFILE_FUNCTION_IN_SESSION( "sess_core_render" );

        if ( !m_hasValidInfo )
            return;

        // (0): keep visible renderables
        auto _renderablesVisible = std::vector< CIRenderable* >();
        for ( auto _renderable : renderables )
            if ( _renderable->visible() )
                _renderablesVisible.push_back( _renderable );

        // (1): frustum culling (if enabled)
        CFrustum _frustum( m_renderOptions.cameraPtr->mat_proj() * m_renderOptions.cameraPtr->mat_view() );
        auto _renderablesInView = std::vector< CIRenderable* >();
        if ( m_renderOptions.useFrustumCulling )
            _collectRenderablesInView( _frustum, m_renderOptions.cullingGeom, _renderablesVisible, _renderablesInView );
        else
            _renderablesInView = _renderablesVisible;

        // (3): group by renderable types (to pass to specific renderers)
        auto _rendMeshesVisible = std::vector< CMesh* >();
        auto _rendMeshesInView  = std::vector< CMesh* >();
        _collectMeshes( _renderablesVisible, _rendMeshesVisible );
        _collectMeshes( _renderablesInView, _rendMeshesInView );

        // (3.5) return if only testing functionality above
        if ( m_renderOptions.mode == eRenderMode::NO_SUBMIT )
            return;

        // (4) submit to specific renderers for them to do extra preparations
        m_rendererMeshes->submit( _rendMeshesVisible, _rendMeshesInView );
    }

    void CMainRenderer::render()
    {
        ENGINE_PROFILE_FUNCTION_IN_SESSION( "sess_core_render" );

        if ( !m_hasValidInfo )
            return;

        if ( m_renderOptions.mode == eRenderMode::NO_SUBMIT )
            return;

        // Start making the actual rendering process (use forward rendering for now)

        /* (1): render pass for shadow mapping (if enabled) ***************************************/
        if ( m_renderOptions.mode == eRenderMode::NORMAL &&
             m_renderOptions.useShadowMapping &&
             m_renderOptions.redrawShadowMap )
        {
            // bind the shadow-map (change render-target)
            m_renderOptions.shadowMapPtr->bind();
            // do the shadow-mapping render pass
            m_rendererMeshes->renderToShadowMap();
            // go back to default render-target
            m_renderOptions.shadowMapPtr->unbind();
        }

        /* (2): start actual render pass for the objects ******************************************/

        /* (2.1): cache previous viewport properties */
        int32 _currentViewport[4];
        glGetIntegerv( GL_VIEWPORT, _currentViewport );
        int32 _prevViewportX = _currentViewport[0];
        int32 _prevViewportY = _currentViewport[1];
        int32 _prevViewportWidth  = _currentViewport[2];
        int32 _prevViewportHeight = _currentViewport[3];

        /* (2.2): setup the requested viewport */
        glViewport( 0, 0, m_renderOptions.viewportWidth, m_renderOptions.viewportHeight );

        /* (2.3): setup render target (if given) */
        if ( m_renderOptions.renderTargetPtr )
            m_renderOptions.renderTargetPtr->bind();

        // prepare for rendering
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        /* (2.4): render skybox first (normal mode only) to play nicely with blending */
        if ( m_renderOptions.mode == eRenderMode::NORMAL && m_renderOptions.useSkybox )
            m_rendererSkybox->render();

        /* (2.5): render the scene according to the render mode requested */
        if ( m_renderOptions.mode == eRenderMode::NORMAL )
        {
            m_rendererMeshes->renderMeshesOpaque();
            if ( m_renderOptions.useBlending )
                m_rendererMeshes->renderMeshesTransparent();
        }
        else if ( m_renderOptions.mode == eRenderMode::DEPTH_ONLY )
        {
            m_rendererMeshes->renderDepthOnly();
        }
        else if ( m_renderOptions.mode == eRenderMode::SEMANTIC_ONLY )
        {
            m_rendererMeshes->renderSemanticOnly();
        }

        /* (2.6): be nice and restore previous viewport */
        glViewport( _prevViewportX, _prevViewportY, _prevViewportWidth, _prevViewportHeight );

        /* (2.7): release custom render target (in case used) */
        if ( m_renderOptions.renderTargetPtr )
            m_renderOptions.renderTargetPtr->unbind();
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

    void CMainRenderer::_collectMeshes( const std::vector< CIRenderable* >& renderables,
                                        std::vector< CMesh* >& meshes )
    {
        for ( auto renderablePtr : renderables )
        {
            if ( renderablePtr->type() == eRenderableType::MESH )
            {
                meshes.push_back( dynamic_cast< CMesh* >( renderablePtr ) );
            }
            else if ( renderablePtr->type() == eRenderableType::MODEL )
            {
                auto _submeshes = dynamic_cast< CModel* >( renderablePtr )->meshes();
                auto _localTransforms = dynamic_cast< CModel* >( renderablePtr )->localTransforms();
                auto _modelWorldTransform = renderablePtr->matModel();
                for ( size_t i = 0; i < _submeshes.size(); i++ )
                {
                    if ( !_submeshes[i]->visible() )
                        continue;

                    // @TODO: Should optimize this part, as we'll do the world-transform computation
                    // again when constructing the model-matrix for this submesh (unpacking and
                    // packing the matrix again )
                    auto _subMeshWorldTransform = _modelWorldTransform * _localTransforms[i];
                    _submeshes[i]->position = CVec3( _subMeshWorldTransform.col( 3 ) );
                    _submeshes[i]->rotation = CMat3( _subMeshWorldTransform );
                    _submeshes[i]->setMaskId( renderablePtr->maskId() );

                    meshes.push_back( _submeshes[i] );
                }
            }
        }
    }

}
