
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

    void CMeshRenderer::submit( const std::vector< CMesh* >& meshesVisible,
                                const std::vector< CMesh* >& meshesInView, 
                                const CRenderOptions& renderOptions )
    {
        if ( !renderOptions.cameraPtr )
            ENGINE_CORE_ASSERT( false, "Mesh-renderer requires a valid camera to render" );

        if ( renderOptions.mode == eRenderMode::NORMAL && !renderOptions.lightPtr )
            ENGINE_CORE_ASSERT( false, "Mesh-renderer requires a valid light to render in normal-mode" );

        // (0): clear buffers to store newly submitted meshes, and store render options
        m_meshesCastShadows.clear();
        m_meshesDontCastShadows.clear();
        m_meshesOpaque.clear();
        m_meshesTransparent.clear();

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
            if ( renderOptions.useBlending && meshPtr->material()->alpha < 1.0f )
                m_meshesTransparent.push_back( meshPtr );
            else
                m_meshesOpaque.push_back( meshPtr );
        }

        // (3): sort the transparent meshes by distance to the camera
        if ( renderOptions.useBlending )
        {
            CMeshComparatorFarthestFromPoint _comparator;
            _comparator.point = renderOptions.cameraPtr->position();

            std::sort( m_meshesTransparent.begin(), m_meshesTransparent.end(), _comparator );
        }

        // (4): prepare context information
        /* render options */
        m_context.useFog         = renderOptions.useFog;
        m_context.useFaceCulling = renderOptions.useFaceCulling;
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

    void CMeshRenderer::render( bool useShadowMapping )
    {
        // (0): collect the meshes according to material type
        auto _meshesOpaqueLambert       = std::vector< CMesh* >();
        auto _meshesOpaquePhong         = std::vector< CMesh* >();
        auto _meshesOpaqueBlinnPhong    = std::vector< CMesh* >();

        _collectMeshesByMaterial( m_meshesOpaque, eMaterialType::LAMBERT, _meshesOpaqueLambert );
        _collectMeshesByMaterial( m_meshesOpaque, eMaterialType::PHONG, _meshesOpaquePhong );
        _collectMeshesByMaterial( m_meshesOpaque, eMaterialType::BLINN_PHONG, _meshesOpaqueBlinnPhong );

        auto _meshesTransparentLambert      = std::vector< CMesh* >();
        auto _meshesTransparentPhong        = std::vector< CMesh* >();
        auto _meshesTransparentBlinnPhong   = std::vector< CMesh* >();

        _collectMeshesByMaterial( m_meshesTransparent, eMaterialType::LAMBERT, _meshesTransparentLambert );
        _collectMeshesByMaterial( m_meshesTransparent, eMaterialType::PHONG, _meshesTransparentPhong );
        _collectMeshesByMaterial( m_meshesTransparent, eMaterialType::BLINN_PHONG, _meshesTransparentBlinnPhong );

        // (1): group meshes that can be face-culled (if enabled)
        auto _meshesOpaqueLambert_faceCull = std::vector< CMesh* >();
        auto _meshesOpaqueLambert_noFaceCull = std::vector< CMesh* >();
        auto _meshesOpaquePhong_faceCull = std::vector< CMesh* >();
        auto _meshesOpaquePhong_noFaceCull = std::vector< CMesh* >();
        auto _meshesOpaqueBlinnPhong_faceCull = std::vector< CMesh* >();
        auto _meshesOpaqueBlinnPhong_noFaceCull = std::vector< CMesh* >();

        if ( m_context.useFaceCulling )
        {
            for ( auto meshPtr : _meshesOpaqueLambert )
            {
                if ( meshPtr->cullFaces )
                    _meshesOpaqueLambert_faceCull.push_back( meshPtr );
                else
                    _meshesOpaqueLambert_noFaceCull.push_back( meshPtr );
            }

            for ( auto meshPtr : _meshesOpaquePhong )
            {
                if ( meshPtr->cullFaces )
                    _meshesOpaquePhong_faceCull.push_back( meshPtr );
                else
                    _meshesOpaquePhong_noFaceCull.push_back( meshPtr );
            }

            for ( auto meshPtr : _meshesOpaqueBlinnPhong )
            {
                if ( meshPtr->cullFaces )
                    _meshesOpaqueBlinnPhong_faceCull.push_back( meshPtr );
                else
                    _meshesOpaqueBlinnPhong_noFaceCull.push_back( meshPtr );
            }
        }

        // (2): render opaque meshes
        if ( m_context.useFaceCulling )
        {
            _renderMeshes( _meshesOpaqueLambert_faceCull, 
                           _meshesOpaqueLambert_noFaceCull,
                           _meshesOpaquePhong_faceCull, 
                           _meshesOpaquePhong_noFaceCull,
                           _meshesOpaqueBlinnPhong_faceCull,
                           _meshesOpaqueBlinnPhong_noFaceCull,
                           useShadowMapping );
        }
        else
        {
            _renderMeshes( std::vector< CMesh* >(), 
                           _meshesOpaqueLambert,
                           std::vector< CMesh* >(), 
                           _meshesOpaquePhong,
                           std::vector< CMesh* >(),
                           _meshesOpaqueBlinnPhong,
                           useShadowMapping );
        }

        // (3): render transparent meshes
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

    void CMeshRenderer::_collectMeshesByMaterial( const std::vector< CMesh* >& meshes,
                                                  const eMaterialType& materialType,
                                                  std::vector< CMesh* >& meshesOfGivenMaterial )
    {
        for ( auto meshPtr : meshes )
            if ( meshPtr->material()->type() == materialType )
                meshesOfGivenMaterial.push_back( meshPtr );
    }

    void CMeshRenderer::_renderMeshes( const std::vector< CMesh* >& meshesLambertWithFaceCulling, 
                                       const std::vector< CMesh* >& meshesLambertWithNoFaceCulling,
                                       const std::vector< CMesh* >& meshesPhongWithFaceCulling, 
                                       const std::vector< CMesh* >& meshesPhongWithNoFaceCulling,
                                       const std::vector< CMesh* >& meshesBlinnPhongWithFaceCulling, 
                                       const std::vector< CMesh* >& meshesBlinnPhongWithNoFaceCulling,
                                       bool renderWithShadows )
    {
        CShader* _shader = nullptr;
        if ( !renderWithShadows && !m_context.useFog ) _shader = m_shaderNoShadowsNoFog;
        if ( !renderWithShadows && m_context.useFog ) _shader = m_shaderNoShadowsFog;
        if ( renderWithShadows && !m_context.useFog ) _shader = m_shaderShadowsNoFog;
        if ( renderWithShadows && m_context.useFog ) _shader = m_shaderShadowsFog;

        // setup render state
        _shader->bind();
        _setupRenderState_camera( _shader );
        _setupRenderState_light( _shader );
        if ( renderWithShadows )
            _setupRenderState_shadows( _shader );
        if ( m_context.useFog )
            _setupRenderState_fog( _shader );

        // configure viewer position (required for specular-calculations)
        _shader->setVec3( "u_viewerPosition", m_context.viewPosition );

        if ( m_context.useFaceCulling )
        {
            glEnable( GL_CULL_FACE );

            // setup common uniforms for lambert materials
            _shader->setInt( "u_material.type", 0 );
            _shader->setInt( "u_material.specularMapActive", 0 );
            _shader->setVec3( "u_material.specular", { 0.0f, 0.0f, 0.0f } );
            _shader->setFloat( "u_material.shininess", 32.0f );
            for ( auto meshPtr : meshesLambertWithFaceCulling )
                _renderMesh_Lambert( meshPtr, _shader );

            // setup common uniforms for phong materials
            _shader->setInt( "u_material.type", 1 );
            for ( auto meshPtr : meshesPhongWithFaceCulling )
                _renderMesh_Phong( meshPtr, _shader );

            // setup common uniforms for blinn-phong materials
            _shader->setInt( "u_material.type", 2 );
            for ( auto meshPtr : meshesBlinnPhongWithFaceCulling )
                _renderMesh_BlinnPhong( meshPtr, _shader );

            glDisable( GL_CULL_FACE );
        }

        // setup common uniforms for lambert materials
        _shader->setInt( "u_material.type", 0 );
        _shader->setInt( "u_material.specularMapActive", 0 );
        for ( auto meshPtr : meshesLambertWithNoFaceCulling )
            _renderMesh_Lambert( meshPtr, _shader );

        // setup common uniforms for phong materials
        _shader->setInt( "u_material.type", 1 );
        for ( auto meshPtr : meshesPhongWithNoFaceCulling )
            _renderMesh_Phong( meshPtr, _shader );

        // setup common uniforms for blinn-phong materials
        _shader->setInt( "u_material.type", 2 );
        for ( auto meshPtr : meshesBlinnPhongWithNoFaceCulling )
            _renderMesh_BlinnPhong( meshPtr, _shader );

        // unbind albedo-map, specular-map and depth-map (end with texture-0 to go back to 
        // default-texture 0 being active, in case someone binds this slot without activating it)
        glActiveTexture( GL_TEXTURE0 + MESH_RENDERER_DEPTH_MAP_SLOT );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glActiveTexture( GL_TEXTURE0 + MESH_RENDERER_SPECULAR_MAP_SLOT );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glActiveTexture( GL_TEXTURE0 + MESH_RENDERER_ALBEDO_MAP_SLOT );
        glBindTexture( GL_TEXTURE_2D, 0 );

        _shader->unbind();
    }

    void CMeshRenderer::_renderMesh_Lambert( CMesh* meshPtr, CShader* shaderPtr )
    {
        // setup material uniforms
        auto _materialPtr = meshPtr->material();
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

    void CMeshRenderer::_renderMesh_Phong( CMesh* meshPtr, CShader* shaderPtr )
    {
        // setup material uniforms
        auto _materialPtr = meshPtr->material();
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

    void CMeshRenderer::_renderMesh_BlinnPhong( CMesh* meshPtr, CShader* shaderPtr )
    {
        // setup material uniforms
        auto _materialPtr = meshPtr->material();
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