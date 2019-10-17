
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

    void CMainRenderer::render( CScene* scenePtr, 
                                CRenderOptions renderOptions )
    {
        // (0): prepare for rendering

        /* (0.1) grab all renderables, setup default render options, and initialize status string */
        auto _renderablesAll = scenePtr->renderables();
        renderOptions.cameraPtr     = ( !renderOptions.cameraPtr ) ? scenePtr->currentCamera() : renderOptions.cameraPtr;
        renderOptions.lightPtr      = ( !renderOptions.lightPtr ) ? scenePtr->mainLight() : renderOptions.lightPtr;
        renderOptions.shadowMapPtr  = ( !renderOptions.shadowMapPtr ) ? m_shadowMap.get() : renderOptions.shadowMapPtr;
        renderOptions.fogPtr        = ( !renderOptions.fogPtr ) ? scenePtr->fog() : renderOptions.fogPtr;
        renderOptions.skyboxPtr     = ( !renderOptions.skyboxPtr ) ? scenePtr->skybox() : renderOptions.skyboxPtr;
        m_status = "renderables         : " + std::to_string( _renderablesAll.size() ) + "\n\r";

        /* (0.2) do some error handling in case some options and some resources don't match */
        if ( renderOptions.useShadowMapping && !renderOptions.shadowMapPtr )
        {
            ENGINE_CORE_WARN( "Renderer was setup to use shadow mapping, but no shadow-map provided" );
            renderOptions.useShadowMapping = false;
        }
        if ( renderOptions.useFog && !renderOptions.fogPtr )
        {
            ENGINE_CORE_WARN( "Renderer was setup to use fog, but no fog-struct provided" );
            renderOptions.useFog = false;
        }
        if ( renderOptions.useSkybox && !renderOptions.skyboxPtr )
        {
            ENGINE_CORE_WARN( "Renderer was setup to use a skybox, but no skybox-struct provided" );
            renderOptions.useSkybox = false;
        }
        if ( !renderOptions.cameraPtr )
        {
            ENGINE_CORE_ERROR( "Renderer requires a camera to render to. Skipping this render pass" );
            return;
        }
        if ( renderOptions.mode == eRenderMode::NORMAL && !renderOptions.lightPtr )
        {
            ENGINE_CORE_ERROR( "Renderer requires a light when using NORMAL mode. Skipping this render pass" );
            return;
        }

        // (1): grab all renderables and keep only visible ones
        auto _renderablesVisible = std::vector< CIRenderable* >();
        for ( auto _renderable : _renderablesAll )
            if ( _renderable->visible() )
                _renderablesVisible.push_back( _renderable );
        m_status += "renderablesVisible : " + std::to_string( _renderablesVisible.size() ) + "\n\r";

        // (2): frustum culling (if enabled)
        CFrustum _frustum( renderOptions.cameraPtr->matProj() * renderOptions.cameraPtr->matView() );
        auto _renderablesInView = std::vector< CIRenderable* >();
        if ( renderOptions.useFrustumCulling )
            _collectRenderablesInView( _frustum, renderOptions.cullingGeom, _renderablesVisible, _renderablesInView );
        else
            _renderablesInView = _renderablesVisible;
        m_status += "renderablesInView  : " + std::to_string( _renderablesInView.size() ) + "\n\r";

        // (3): group by renderable types (to pass to specific renderers)
        auto _rendMeshesVisible = std::vector< CMesh* >();
        auto _rendMeshesInView  = std::vector< CMesh* >();
        _collectMeshes( _renderablesVisible, _rendMeshesVisible );
        _collectMeshes( _renderablesInView, _rendMeshesInView );
        m_status += "meshes-visible : " + std::to_string( _rendMeshesVisible.size() ) + "\n\r";
        m_status += "meshes-in-view : " + std::to_string( _rendMeshesInView.size() ) + "\n\r";

        // (3.5) return if only testing functionality above
        if ( renderOptions.mode == eRenderMode::NO_SUBMIT )
            return;

        // (4) submit to specific renderers for them to do extra preparations
        m_rendererMeshes->submit( _rendMeshesVisible, _rendMeshesInView, renderOptions );
        if ( renderOptions.useSkybox )
            m_rendererSkybox->submit( renderOptions );

        // (5) start making the actual rendering process (use forward rendering for now)

        /* (5.1) render pass for shadow mapping (if enabled) */
        if ( renderOptions.mode == eRenderMode::NORMAL && 
             renderOptions.useShadowMapping && 
             renderOptions.redrawShadowMap )
        {
            // configure the light-space from configuration from user
            renderOptions.shadowMapPtr->setup( renderOptions.shadowMapRangeConfig );
            // bind the shadow-map (change render-target)
            renderOptions.shadowMapPtr->bind();
            // do the shadow-mapping render pass
            m_rendererMeshes->renderToShadowMap();
            // go back to default render-target
            renderOptions.shadowMapPtr->unbind();
        }

        // cache previous viewport properties
        int32 _currentViewport[4];
        glGetIntegerv( GL_VIEWPORT, _currentViewport );
        int32 _prevViewportX = _currentViewport[0];
        int32 _prevViewportY = _currentViewport[1];
        int32 _prevViewportWidth  = _currentViewport[2];
        int32 _prevViewportHeight = _currentViewport[3];

        // setup the requested viewport
        glViewport( 0, 0, renderOptions.viewportWidth, renderOptions.viewportHeight );

        /* (5.2) setup render target if given*/
        if ( renderOptions.renderTargetPtr )
            renderOptions.renderTargetPtr->bind();

        // prepare for rendering
        glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT );

        /* (5.3) render pass for the scene itself */
        if ( renderOptions.mode == eRenderMode::NORMAL )
            m_rendererMeshes->render( renderOptions.useShadowMapping );

        else if ( renderOptions.mode == eRenderMode::DEPTH_ONLY )
            m_rendererMeshes->renderDepthOnly();

        else if ( renderOptions.mode == eRenderMode::SEMANTIC_ONLY )
            m_rendererMeshes->renderSemanticOnly();

        /* (5.4) render pass for the skybox */
        if ( renderOptions.useSkybox )
            m_rendererSkybox->render();

        // restore previous viewport
        glViewport( _prevViewportX, _prevViewportY, _prevViewportWidth, _prevViewportHeight );

        /* (5.5) release custom render target in case used */
        if ( renderOptions.renderTargetPtr )
            renderOptions.renderTargetPtr->unbind();
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
                    _submeshes[i]->position = _subMeshWorldTransform.getPosition();
                    _submeshes[i]->rotation = _subMeshWorldTransform.getRotation();
                    _submeshes[i]->setMaskId( renderablePtr->maskId() );

                    meshes.push_back( _submeshes[i] );
                }
            }
        }
    }

}