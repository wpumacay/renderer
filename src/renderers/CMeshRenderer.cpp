
#include <renderers/CMeshRenderer.h>

namespace engine
{

    CMeshRenderer::CMeshRenderer()
    {
        m_shaderNoShadowsNoFog = CShaderManager::GetCachedShader( "engine_no_shadows_no_fog" );
        m_shaderNoShadowsFog = CShaderManager::GetCachedShader( "engine_no_shadows_fog" );
        m_shaderShadowsNoFog = CShaderManager::GetCachedShader( "engine_shadows_no_fog" );
        m_shaderShadowsFog = CShaderManager::GetCachedShader( "engine_shadows_fog" );

        m_shaderShadowMapping   = CShaderManager::GetCachedShader( "engine_shadow_mapping" );
        m_shaderDepthView       = CShaderManager::GetCachedShader( "engine_render_depth_view" );
        m_shaderSemanticView    = CShaderManager::GetCachedShader( "engine_render_semantic_view" );
    }

    CMeshRenderer::~CMeshRenderer()
    {
        m_shaderNoShadowsNoFog = nullptr;
        m_shaderNoShadowsFog = nullptr;
        m_shaderShadowsNoFog = nullptr;
        m_shaderShadowsFog = nullptr;

        m_shaderShadowMapping = nullptr;
        m_shaderDepthView = nullptr;
        m_shaderSemanticView = nullptr;
    }

    void CMeshRenderer::begin( const CRenderOptions& renderOptions )
    {
        // (0): sanity checks: make sure we have the right resources
        if ( !renderOptions.cameraPtr )
            ENGINE_CORE_ASSERT( false, "Mesh-renderer requires a valid camera to render" );

        if ( renderOptions.mode == eRenderMode::NORMAL && !renderOptions.lightPtr )
            ENGINE_CORE_ASSERT( false, "Mesh-renderer requires a valid light to render in normal-mode" );

        /* (1): Prepare context information from render options. We're grabbing required info to
                potentially separate rendering in another thread without touching high-level 
                objects like lights, cameras, etc., which the user might still update during simulation */

        /* render options */
        m_context.useFog            = renderOptions.useFog;
        m_context.useFaceCulling    = renderOptions.useFaceCulling;
        m_context.useBlending       = renderOptions.useBlending;
        m_context.useShadowMapping  = renderOptions.useShadowMapping;
        /* camera-information */
        m_context.viewMatrix                = renderOptions.cameraPtr->matView();
        m_context.projMatrix                = renderOptions.cameraPtr->matProj();
        m_context.viewPosition              = renderOptions.cameraPtr->position();
        /* light-information */
        if ( renderOptions.lightPtr )
        {
            auto _lightPtr = renderOptions.lightPtr;
            m_context.lightType                 = _lightPtr->type();
            m_context.lightAmbient              = _lightPtr->ambient;
            m_context.lightDiffuse              = _lightPtr->diffuse;
            m_context.lightSpecular             = _lightPtr->specular;
            m_context.lightIntensity            = _lightPtr->intensity;
            m_context.lightPosition             = _lightPtr->position;
            m_context.lightDirection            = CVec3::normalize( _lightPtr->direction );
            m_context.lightInnerCutoffCos       = std::cos( _lightPtr->innerCutoff );
            m_context.lightOuterCutoffCos       = std::cos( _lightPtr->outerCutoff );
            m_context.lightAttnK0               = _lightPtr->atnConstant;
            m_context.lightAttnK1               = _lightPtr->atnLinear;
            m_context.lightAttnK2               = _lightPtr->atnQuadratic;
        }
        /* shadow-mapping information */
        if ( renderOptions.useShadowMapping && renderOptions.shadowMapPtr )
        {
            auto _shadowMapPtr = renderOptions.shadowMapPtr;
            m_context.shadowMappingViewMatrix   = _shadowMapPtr->lightSpaceMatView();
            m_context.shadowMappingProjMatrix   = _shadowMapPtr->lightSpaceMatProj();
            m_context.shadowMappingSize         = _shadowMapPtr->width();
            m_context.shadowMappingTextureId    = _shadowMapPtr->frameBuffer()->getTextureAttachment( "shadow_depth_attachment" )->openglId();
            m_context.shadowMappingPCFcount     = renderOptions.pcfCount;
        }
        /* fog information */
        if ( renderOptions.useFog && renderOptions.fogPtr )
        {
            auto _fogPtr = renderOptions.fogPtr;
            m_context.fogType       = ( _fogPtr->type() == eFogType::EXPONENTIAL ) ? 1 : 0;
            m_context.fogColor      = _fogPtr->color;
            m_context.fogDensity    = _fogPtr->density;
            m_context.fogGradient   = _fogPtr->gradient;
            m_context.fogDistStart  = _fogPtr->distStart;
            m_context.fogDistEnd    = _fogPtr->distEnd;
        }
        /* depth-view information */
        if ( renderOptions.mode == eRenderMode::DEPTH_ONLY )
        {
            m_context.depthViewZnear        = renderOptions.cameraPtr->projData().zNear;
            m_context.depthViewZfar         = renderOptions.cameraPtr->projData().zFar;
            m_context.depthViewZmin         = renderOptions.depthViewZmin;
            m_context.depthViewZmax         = renderOptions.depthViewZmax;
            m_context.depthViewZminColor    = renderOptions.depthViewZminColor;
            m_context.depthViewZmaxColor    = renderOptions.depthViewZmaxColor;
        }
        /* semantic-view information */
        if ( renderOptions.mode == eRenderMode::SEMANTIC_ONLY )
        {
            m_context.semanticViewIdMap = renderOptions.semanticViewIdMap;
        }

        /* (2): clear buffers to store newly submitted meshes, and store render options */
        m_meshesCastShadows.clear();
        m_meshesDontCastShadows.clear();

        m_meshesOpaque.clear();
        m_meshesTransparent.clear();
    }

    void CMeshRenderer::submit( const std::vector< CMesh* >& meshesVisible,
                                const std::vector< CMesh* >& meshesInView )
    {
        // (1): separate the ones that cast shadows from the ones that don't (for shadow-map pass)
        //      > Note: check all visible meshes, as they could cast shadow even outside view frustum
        for ( auto meshPtr : meshesVisible )
        {
            if ( meshPtr->castsShadows )
                m_meshesCastShadows.push_back( meshPtr );
            else 
                m_meshesDontCastShadows.push_back( meshPtr );
        }

        // (2): separate the ones that are opaque from the ones that are transparent (for blending)
        //      > Note: check only in-view meshes, as they are going to be rendered w.r.t. our view
        for ( auto meshPtr : meshesInView )
        {
            if ( m_context.useBlending && meshPtr->material()->transparent && meshPtr->material()->alpha < 1.0f )
                m_meshesTransparent.push_back( meshPtr );
            else
                m_meshesOpaque.push_back( meshPtr );
        }
    }

    void CMeshRenderer::renderToShadowMap()
    {
        m_shaderShadowMapping->bind();
        m_shaderShadowMapping->setMat4( "u_lightSpaceViewProjMatrix", m_context.shadowMappingProjMatrix *
                                                                      m_context.shadowMappingViewMatrix );
        for ( auto meshPtr : m_meshesCastShadows )
        {
            m_shaderShadowMapping->setMat4( "u_modelMatrix", meshPtr->matModel() );
            
            meshPtr->vertexArray()->bind();

            glDrawElements( GL_TRIANGLES,
                            meshPtr->vertexArray()->indexBuffer()->count(),
                            GL_UNSIGNED_INT, 0 );

            meshPtr->vertexArray()->unbind();
        }
        m_shaderShadowMapping->unbind();
    }

    void CMeshRenderer::renderMeshesOpaque()
    {
        // if there are no transparent objects, just skip to avoid setting render state
        if ( m_meshesOpaque.size() < 1 )
            return;

        // (0): define which shader we'll be using for the render passes
        CShader* _shader = nullptr;
        if ( !m_context.useShadowMapping && !m_context.useFog ) _shader = m_shaderNoShadowsNoFog;
        if ( !m_context.useShadowMapping && m_context.useFog ) _shader = m_shaderNoShadowsFog;
        if ( m_context.useShadowMapping && !m_context.useFog ) _shader = m_shaderShadowsNoFog;
        if ( m_context.useShadowMapping && m_context.useFog ) _shader = m_shaderShadowsFog;

        // (1): render opaque meshes according to face-culling options
        if ( m_context.useFaceCulling )
        {
            // separate meshes according to face-culling settings
            auto _meshesOpaque_faceCull = std::vector< CMesh* >();
            auto _meshesOpaque_noFaceCull = std::vector< CMesh* >();

            for ( auto meshPtr : m_meshesOpaque )
            {
                if ( meshPtr->cullFaces )
                    _meshesOpaque_faceCull.push_back( meshPtr );
                else
                    _meshesOpaque_noFaceCull.push_back( meshPtr );
            }

            _renderMeshes( _shader,
                           _meshesOpaque_faceCull, 
                           _meshesOpaque_noFaceCull,
                           false /* don't use blending for this pass */);
        }
        else
        {
            _renderMeshes( _shader,
                           std::vector< CMesh* >(), 
                           m_meshesOpaque,
                           false /* don't use blending for this pass */);
        }
    }

    void CMeshRenderer::renderMeshesTransparent()
    {
        if ( !m_context.useBlending )
        {
            ENGINE_CORE_WARN( "Tried rendering transparent objects without configuring blending" );
            return;
        }

        // if there are no transparent objects, just skip to avoid setting render state
        if ( m_meshesTransparent.size() < 1 )
            return;

        // (0): define which shader we'll be using for the render passes
        CShader* _shader = nullptr;
        if ( !m_context.useShadowMapping && !m_context.useFog ) _shader = m_shaderNoShadowsNoFog;
        if ( !m_context.useShadowMapping && m_context.useFog ) _shader = m_shaderNoShadowsFog;
        if ( m_context.useShadowMapping && !m_context.useFog ) _shader = m_shaderShadowsNoFog;
        if ( m_context.useShadowMapping && m_context.useFog ) _shader = m_shaderShadowsFog;

        // (1): sort the transparent meshes by distance to the camera
        CMeshComparatorFarthestFromPoint _comparator;
        _comparator.point = m_context.viewPosition;

        std::sort( m_meshesTransparent.begin(), m_meshesTransparent.end(), _comparator );

        // (2): do the actual render call (reusing previously set render-state)
        _renderMeshes( _shader,
                       std::vector< CMesh* >(),
                       m_meshesTransparent,
                       true /* use blending for this pass */);
    }

    void CMeshRenderer::renderDepthOnly()
    {
        m_shaderDepthView->bind();
        m_shaderDepthView->setMat4( "u_viewProjMatrix", m_context.projMatrix * m_context.viewMatrix );
        m_shaderDepthView->setFloat( "u_znear", m_context.depthViewZnear );
        m_shaderDepthView->setFloat( "u_zfar", m_context.depthViewZfar );
        m_shaderDepthView->setFloat( "u_zmin", m_context.depthViewZmin );
        m_shaderDepthView->setFloat( "u_zmax", m_context.depthViewZmax );
        m_shaderDepthView->setVec3( "u_zminColor", m_context.depthViewZminColor );
        m_shaderDepthView->setVec3( "u_zmaxColor", m_context.depthViewZmaxColor );

        auto _meshesToRender = std::vector< CMesh* >();
        _meshesToRender.insert( _meshesToRender.end(), m_meshesOpaque.begin(), m_meshesOpaque.end() );
        _meshesToRender.insert( _meshesToRender.end(), m_meshesTransparent.begin(), m_meshesTransparent.end() );

        for ( auto meshPtr : _meshesToRender )
        {
            m_shaderDepthView->setMat4( "u_modelMatrix", meshPtr->matModel() );

            auto _vao = meshPtr->vertexArray();
            auto _ibo = _vao->indexBuffer();

            _vao->bind();
            glDrawElements( GL_TRIANGLES, _ibo->count(), GL_UNSIGNED_INT, 0 );
            _vao->unbind();
        }

        m_shaderDepthView->unbind();
    }

    void CMeshRenderer::renderSemanticOnly()
    {
        m_shaderSemanticView->bind();
        m_shaderSemanticView->setMat4( "u_viewProjMatrix", m_context.projMatrix * m_context.viewMatrix );

        auto _meshesToRender = std::vector< CMesh* >();
        _meshesToRender.insert( _meshesToRender.end(), m_meshesOpaque.begin(), m_meshesOpaque.end() );
        _meshesToRender.insert( _meshesToRender.end(), m_meshesTransparent.begin(), m_meshesTransparent.end() );

        for ( auto meshPtr : _meshesToRender )
        {
            if ( m_context.semanticViewIdMap.find( meshPtr->maskId() ) != m_context.semanticViewIdMap.end() )
            {
                m_shaderSemanticView->setVec3( "u_maskColor", m_context.semanticViewIdMap[meshPtr->maskId()] );
            }
            else if ( m_cachedRandomColors.find( meshPtr->maskId() ) != m_cachedRandomColors.end() )
            {
                m_shaderSemanticView->setVec3( "u_maskColor", m_cachedRandomColors[meshPtr->maskId()] );
            }
            else
            {
                CVec3 _maskColor = { m_randDist( m_randGen ), m_randDist( m_randGen ), m_randDist( m_randGen ) };
                m_cachedRandomColors[meshPtr->maskId()] = _maskColor;
                m_shaderSemanticView->setVec3( "u_maskColor", _maskColor );
            }

            m_shaderSemanticView->setMat4( "u_modelMatrix", meshPtr->matModel() );

            auto _vao = meshPtr->vertexArray();
            auto _ibo = _vao->indexBuffer();

            _vao->bind();
            glDrawElements( GL_TRIANGLES, _ibo->count(), GL_UNSIGNED_INT, 0 );
            _vao->unbind();
        }

        m_shaderSemanticView->unbind();
    }

    void CMeshRenderer::_renderMeshes( CShader* shaderPtr,
                                       const std::vector< CMesh* >& meshesWithFaceCulling, 
                                       const std::vector< CMesh* >& meshesWithNoFaceCulling,
                                       bool renderWithBlending )
    {
        // setup render state (if required for this pass)
        shaderPtr->bind();
        _setupRenderState_camera( shaderPtr );
        _setupRenderState_light( shaderPtr );
        if ( m_context.useShadowMapping )
            _setupRenderState_shadows( shaderPtr );
        if ( m_context.useFog )
            _setupRenderState_fog( shaderPtr );

        // configure viewer position (required for specular-calculations)
        shaderPtr->setVec3( "u_viewerPosition", m_context.viewPosition );

        if ( renderWithBlending )
        {
            glEnable( GL_BLEND );
            glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
            // @hack: disable writing to depthbuffer to avoid artifacts :(
            glDepthMask( GL_FALSE );
            // @hack: culling back-faces for blending to avoid artifacts :(
            glEnable( GL_CULL_FACE );
            shaderPtr->setInt( "u_material.transparent", 1 );
        }
        else
        {
            shaderPtr->setInt( "u_material.transparent", 0 );
        }

        // render meshes that require face-culling
        if ( m_context.useFaceCulling && ( meshesWithFaceCulling.size() > 0 ) )
        {
            glEnable( GL_CULL_FACE );

            for ( auto meshPtr : meshesWithFaceCulling )
                _renderMesh( shaderPtr, meshPtr );

            glDisable( GL_CULL_FACE );
        }

        // render meshes that don't require face-culling
        for ( auto meshPtr : meshesWithNoFaceCulling )
        {
            if ( renderWithBlending ) 
                shaderPtr->setFloat( "u_material.alpha", meshPtr->material()->alpha );
            _renderMesh( shaderPtr, meshPtr );
        }

        // unbind albedo-map, specular-map and depth-map (end with texture-0 to go back to 
        // default-texture 0 being active, in case someone binds this slot without activating it)
        glActiveTexture( GL_TEXTURE0 + MESH_RENDERER_DEPTH_MAP_SLOT );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glActiveTexture( GL_TEXTURE0 + MESH_RENDERER_SPECULAR_MAP_SLOT );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glActiveTexture( GL_TEXTURE0 + MESH_RENDERER_ALBEDO_MAP_SLOT );
        glBindTexture( GL_TEXTURE_2D, 0 );

        shaderPtr->unbind();

        if ( renderWithBlending )
        {
            glDisable( GL_CULL_FACE );
            glDepthMask( GL_TRUE );
            glDisable( GL_BLEND );
        }
    }

    void CMeshRenderer::_renderMesh( CShader* shaderPtr, CMesh* meshPtr )
    {
        // setup material uniforms
        auto _materialPtr = meshPtr->material();
        if ( _materialPtr->type() == eMaterialType::LAMBERT )
        {
            shaderPtr->setInt( "u_material.type", 0 );
            shaderPtr->setVec3( "u_material.ambient", _materialPtr->ambient );
            shaderPtr->setVec3( "u_material.diffuse", _materialPtr->diffuse );

            if ( _materialPtr->albedoMap() )
            {
                shaderPtr->setInt( "u_material.albedoMapActive", 1 );
                shaderPtr->setInt( "u_material.albedoMap", MESH_RENDERER_ALBEDO_MAP_SLOT );
                glActiveTexture( GL_TEXTURE0 + MESH_RENDERER_ALBEDO_MAP_SLOT );
                glBindTexture( GL_TEXTURE_2D, _materialPtr->albedoMap()->openglId() );
            }
            else
            {
                shaderPtr->setInt( "u_material.albedoMapActive", 0 );
            }

            // no specular-maps in lambert materials
            shaderPtr->setInt( "u_material.specularMapActive", 0 );
        }
        else if ( _materialPtr->type() == eMaterialType::PHONG ||
                  _materialPtr->type() == eMaterialType::BLINN_PHONG )
        {
            shaderPtr->setInt( "u_material.type", ( _materialPtr->type() == eMaterialType::PHONG ) ? 1 : 2 );
            shaderPtr->setVec3( "u_material.ambient", _materialPtr->ambient );
            shaderPtr->setVec3( "u_material.diffuse", _materialPtr->diffuse );
            shaderPtr->setVec3( "u_material.specular", _materialPtr->specular );
            shaderPtr->setFloat( "u_material.shininess", _materialPtr->shininess );

            if ( _materialPtr->albedoMap() )
            {
                shaderPtr->setInt( "u_material.albedoMapActive", 1 );
                shaderPtr->setInt( "u_material.albedoMap", MESH_RENDERER_ALBEDO_MAP_SLOT );
                glActiveTexture( GL_TEXTURE0 + MESH_RENDERER_ALBEDO_MAP_SLOT );
                glBindTexture( GL_TEXTURE_2D, _materialPtr->albedoMap()->openglId() );
            }
            else
            {
                shaderPtr->setInt( "u_material.albedoMapActive", 0 );
            }

            if ( _materialPtr->specularMap() )
            {
                shaderPtr->setInt( "u_material.specularMapActive", 1 );
                shaderPtr->setInt( "u_material.specularMap", MESH_RENDERER_SPECULAR_MAP_SLOT );
                glActiveTexture( GL_TEXTURE0 + MESH_RENDERER_SPECULAR_MAP_SLOT );
                glBindTexture( GL_TEXTURE_2D, _materialPtr->specularMap()->openglId() );
            }
            else
            {
                shaderPtr->setInt( "u_material.specularMapActive", 0 );
            }
        }

        // setup mesh transforms-uniforms
        auto _matModel = meshPtr->matModel();
        shaderPtr->setMat4( "u_modelMatrix", _matModel );
        shaderPtr->setMat4( "u_normalMatrix", ( _matModel.inverse() ).transpose() );

        // render the mesh ************************************************
        auto _vao = meshPtr->vertexArray();
        auto _ibo = _vao->indexBuffer();

        if ( meshPtr->wireframe() )
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        _vao->bind();
        glDrawElements( GL_TRIANGLES, _ibo->count(), GL_UNSIGNED_INT, 0 );
        _vao->unbind();

        if ( meshPtr->wireframe() )
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        //*****************************************************************
    }

    void CMeshRenderer::_setupRenderState_camera( CShader* shaderPtr )
    {
        if ( m_context.useFog )
            shaderPtr->setMat4( "u_viewMatrix", m_context.viewMatrix );

        shaderPtr->setMat4( "u_viewProjMatrix", m_context.projMatrix * 
                                                m_context.viewMatrix );
    }

    void CMeshRenderer::_setupRenderState_light( CShader* shaderPtr )
    {
        // disable all light-types
        shaderPtr->setInt( "u_directionalLight.enabled", 0 );
        shaderPtr->setInt( "u_pointLight.enabled", 0 );
        shaderPtr->setInt( "u_spotLight.enabled", 0 );

        // setup appropriate light uniforms given the light-type
        if ( m_context.lightType == eLightType::DIRECTIONAL )
        {
            shaderPtr->setInt( "u_directionalLight.enabled", 1 );
            shaderPtr->setVec3( "u_directionalLight.ambient", m_context.lightAmbient );
            shaderPtr->setVec3( "u_directionalLight.diffuse", m_context.lightDiffuse );
            shaderPtr->setVec3( "u_directionalLight.specular", m_context.lightSpecular );
            shaderPtr->setFloat( "u_directionalLight.intensity", m_context.lightIntensity );
            shaderPtr->setVec3( "u_directionalLight.direction", m_context.lightDirection );
        }
        else if ( m_context.lightType == eLightType::POINT )
        {
            shaderPtr->setInt( "u_pointLight.enabled", 1 );
            shaderPtr->setVec3( "u_pointLight.ambient", m_context.lightAmbient );
            shaderPtr->setVec3( "u_pointLight.diffuse", m_context.lightDiffuse );
            shaderPtr->setVec3( "u_pointLight.specular", m_context.lightSpecular );
            shaderPtr->setFloat( "u_pointLight.intensity", m_context.lightIntensity );
            shaderPtr->setVec3( "u_pointLight.position", m_context.lightPosition );
            shaderPtr->setFloat( "u_pointLight.attnk0", m_context.lightAttnK0 );
            shaderPtr->setFloat( "u_pointLight.attnk1", m_context.lightAttnK1 );
            shaderPtr->setFloat( "u_pointLight.attnk2", m_context.lightAttnK2 );
        }
        else if ( m_context.lightType == eLightType::SPOT )
        {
            shaderPtr->setInt( "u_spotLight.enabled", 1 );
            shaderPtr->setVec3( "u_spotLight.ambient", m_context.lightAmbient );
            shaderPtr->setVec3( "u_spotLight.diffuse", m_context.lightDiffuse );
            shaderPtr->setVec3( "u_spotLight.specular", m_context.lightSpecular );
            shaderPtr->setFloat( "u_spotLight.intensity", m_context.lightIntensity );
            shaderPtr->setVec3( "u_spotLight.position", m_context.lightPosition );
            shaderPtr->setFloat( "u_spotLight.attnk0", m_context.lightAttnK0 );
            shaderPtr->setFloat( "u_spotLight.attnk1", m_context.lightAttnK1 );
            shaderPtr->setFloat( "u_spotLight.attnk2", m_context.lightAttnK2 );
            shaderPtr->setVec3( "u_spotLight.direction", m_context.lightDirection );
            shaderPtr->setFloat( "u_spotLight.innerCutoffCos", m_context.lightInnerCutoffCos );
            shaderPtr->setFloat( "u_spotLight.outerCutoffCos", m_context.lightOuterCutoffCos );
        }
    }

    void CMeshRenderer::_setupRenderState_shadows( CShader* shaderPtr )
    {
        shaderPtr->setMat4( "u_viewProjLightSpaceMatrix", m_context.shadowMappingProjMatrix *
                                                          m_context.shadowMappingViewMatrix );

        shaderPtr->setInt( "u_shadowMap.size", m_context.shadowMappingSize );
        shaderPtr->setInt( "u_shadowMap.pcfCount", m_context.shadowMappingPCFcount );
        shaderPtr->setInt( "u_shadowMap.depthMap", MESH_RENDERER_DEPTH_MAP_SLOT );
        glActiveTexture( GL_TEXTURE0 + MESH_RENDERER_DEPTH_MAP_SLOT );
        glBindTexture( GL_TEXTURE_2D, m_context.shadowMappingTextureId );
    }

    void CMeshRenderer::_setupRenderState_fog( CShader* shaderPtr )
    {
        shaderPtr->setInt( "u_fog.type", m_context.fogType );
        shaderPtr->setVec3( "u_fog.color", m_context.fogColor );
        shaderPtr->setFloat( "u_fog.density", m_context.fogDensity );
        shaderPtr->setFloat( "u_fog.gradient", m_context.fogGradient );
        shaderPtr->setFloat( "u_fog.distStart", m_context.fogDistStart );
        shaderPtr->setFloat( "u_fog.distEnd", m_context.fogDistEnd );
    }
}