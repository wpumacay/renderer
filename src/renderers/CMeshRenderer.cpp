
#include <renderers/CMeshRenderer.h>

namespace engine
{

    CMeshRenderer::CMeshRenderer()
    {

    }

    CMeshRenderer::~CMeshRenderer()
    {

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
        if ( renderOptions.shadowMapPtr )
        {
            auto _shadowMapPtr = renderOptions.shadowMapPtr;
            m_context.shadowMappingViewMatrix   = _shadowMapPtr->lightSpaceMatView();
            m_context.shadowMappingProjMatrix   = _shadowMapPtr->lightSpaceMatProj();
            m_context.shadowMappingSize         = _shadowMapPtr->width();
            m_context.shadowMappingTextureId    = _shadowMapPtr->frameBuffer()->getTextureAttachment( "shadow_depth_attachment" )->openglId();
            m_context.shadowMappingPCFcount     = renderOptions.pcfCount;
        }
        /* fog information */
        if ( renderOptions.fogPtr )
        {
            auto _fogPtr = renderOptions.fogPtr;
            m_context.fogType       = ( _fogPtr->type() == eFogType::EXPONENTIAL ) ? 1 : 0;
            m_context.fogEnabled    = ( renderOptions.useFog ) ? 1 : 0;
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
        auto _shaderShadowMapping = CShaderManager::GetCachedShader( "engine_shadow_mapping" );

        _shaderShadowMapping->bind();
        _shaderShadowMapping->setMat4( "u_lightSpaceViewProjMatrix", m_context.shadowMappingProjMatrix *
                                                                     m_context.shadowMappingViewMatrix );
        for ( auto meshPtr : m_meshesCastShadows )
        {
            _shaderShadowMapping->setMat4( "u_modelMatrix", meshPtr->matModel() );
            
            meshPtr->vertexArray()->bind();

            glDrawElements( GL_TRIANGLES,
                            meshPtr->vertexArray()->indexBuffer()->count(),
                            GL_UNSIGNED_INT, 0 );

            meshPtr->vertexArray()->unbind();
        }
        _shaderShadowMapping->unbind();
    }

    void CMeshRenderer::renderWithShadowMap()
    {
        // (0): collect the meshes according to material type
        auto _meshesOpaqueLambert       = std::vector< CMesh* >();
        auto _meshesOpaquePhong         = std::vector< CMesh* >();
        // auto _meshesOpaqueBlinnPhong    = std::vector< CMesh* >();

        _collectMeshesByMaterial< CLambertMaterial >( m_meshesOpaque, _meshesOpaqueLambert );
        _collectMeshesByMaterial< CPhongMaterial >( m_meshesOpaque, _meshesOpaquePhong );
        // _collectMeshesByMaterial< CBlinnPhongMaterial >( m_meshesOpaque, _meshesOpaqueBlinnPhong );

        auto _meshesTransparentLambert      = std::vector< CMesh* >();
        auto _meshesTransparentPhong        = std::vector< CMesh* >();
        // auto _meshesTransparentBlinnPhong   = std::vector< CMesh* >();

        _collectMeshesByMaterial< CLambertMaterial >( m_meshesTransparent, _meshesTransparentLambert );
        _collectMeshesByMaterial< CPhongMaterial >( m_meshesTransparent, _meshesTransparentPhong );
        // _collectMeshesByMaterial< CBlinnPhongMaterial >( m_meshesTransparent, _meshesTransparentBlinnPhong );

        // (1): render opaque meshes
        _render_Lambert( _meshesOpaqueLambert, true );
        _render_Phong( _meshesOpaquePhong, true );
        //// _render_BlinnPhong( _meshesOpaqueBlinnPhong, true );
    }

    void CMeshRenderer::renderWithoutShadowMap()
    {
        // (0): collect the meshes according to material type
        auto _meshesOpaqueLambert     = std::vector< CMesh* >();
        auto _meshesOpaquePhong       = std::vector< CMesh* >();
        // auto _meshesOpaqueBlinnPhong  = std::vector< CMesh* >();

        _collectMeshesByMaterial< CLambertMaterial >( m_meshesOpaque, _meshesOpaqueLambert );
        _collectMeshesByMaterial< CPhongMaterial >( m_meshesOpaque, _meshesOpaquePhong );
        // _collectMeshesByMaterial< CBlinnPhongMaterial >( m_meshesOpaque, _meshesOpaqueBlinnPhong );

        auto _meshesTransparentLambert     = std::vector< CMesh* >();
        auto _meshesTransparentPhong       = std::vector< CMesh* >();
        // auto _meshesTransparentBlinnPhong  = std::vector< CMesh* >();

        _collectMeshesByMaterial< CLambertMaterial >( m_meshesTransparent, _meshesTransparentLambert );
        _collectMeshesByMaterial< CPhongMaterial >( m_meshesTransparent, _meshesTransparentPhong );
        // _collectMeshesByMaterial< CBlinnPhongMaterial >( m_meshesTransparent, _meshesTransparentBlinnPhong );

        // (1): render opaque meshes
        _render_Lambert( _meshesOpaqueLambert, false );
        _render_Phong( _meshesOpaquePhong, false );
        //// _render_BlinnPhong( _meshesOpaqueBlinnPhong, false );
    }

    void CMeshRenderer::renderDepthOnly()
    {
        auto _shaderDepthView = CShaderManager::GetCachedShader( "engine_render_depth_view" );

        _shaderDepthView->bind();
        _shaderDepthView->setMat4( "u_viewProjMatrix", m_context.projMatrix * m_context.viewMatrix );
        _shaderDepthView->setFloat( "u_znear", m_context.depthViewZnear );
        _shaderDepthView->setFloat( "u_zfar", m_context.depthViewZfar );
        _shaderDepthView->setFloat( "u_zmin", m_context.depthViewZmin );
        _shaderDepthView->setFloat( "u_zmax", m_context.depthViewZmax );
        _shaderDepthView->setVec3( "u_zminColor", m_context.depthViewZminColor );
        _shaderDepthView->setVec3( "u_zmaxColor", m_context.depthViewZmaxColor );

        for ( auto meshPtr : m_meshesOpaque )
        {
            _shaderDepthView->setMat4( "u_modelMatrix", meshPtr->matModel() );

            auto _vao = meshPtr->vertexArray();
            auto _ibo = _vao->indexBuffer();

            _vao->bind();
            glDrawElements( GL_TRIANGLES, _ibo->count(), GL_UNSIGNED_INT, 0 );
            _vao->unbind();
        }

        _shaderDepthView->unbind();
    }

    void CMeshRenderer::renderSemanticOnly()
    {
        auto _shaderSemanticView = CShaderManager::GetCachedShader( "engine_render_semantic_view" );

        _shaderSemanticView->bind();
        _shaderSemanticView->setMat4( "u_viewProjMatrix", m_context.projMatrix * m_context.viewMatrix );

        for ( auto meshPtr : m_meshesOpaque )
        {
            if ( m_context.semanticViewIdMap.find( meshPtr->maskId() ) != m_context.semanticViewIdMap.end() )
            {
                _shaderSemanticView->setVec3( "u_maskColor", m_context.semanticViewIdMap[meshPtr->maskId()] );
            }
            else if ( m_cachedRandomColors.find( meshPtr->maskId() ) != m_cachedRandomColors.end() )
            {
                _shaderSemanticView->setVec3( "u_maskColor", m_cachedRandomColors[meshPtr->maskId()] );
            }
            else
            {
                CVec3 _maskColor = { m_randDist( m_randGen ), m_randDist( m_randGen ), m_randDist( m_randGen ) };
                m_cachedRandomColors[meshPtr->maskId()] = _maskColor;
                _shaderSemanticView->setVec3( "u_maskColor", _maskColor );
            }

            _shaderSemanticView->setMat4( "u_modelMatrix", meshPtr->matModel() );

            auto _vao = meshPtr->vertexArray();
            auto _ibo = _vao->indexBuffer();

            _vao->bind();
            glDrawElements( GL_TRIANGLES, _ibo->count(), GL_UNSIGNED_INT, 0 );
            _vao->unbind();
        }

        _shaderSemanticView->unbind();
    }

    void CMeshRenderer::_render_Lambert( const std::vector< CMesh* >& meshes, bool renderWithShadows )
    {
        auto _shaderLambert = CShaderManager::GetCachedShader( "engine_lambert_shadows" );

        // setup render state
        _shaderLambert->bind();
        _setupRenderState_camera( _shaderLambert );
        _setupRenderState_light( _shaderLambert );
        if ( renderWithShadows )
            _setupRenderState_shadows( _shaderLambert );
        if ( m_context.fogEnabled == 1 )
            _setupRenderState_fog( _shaderLambert );

        for ( auto meshPtr : meshes )
        {
            // setup material uniforms
            auto _materialPtr = meshPtr->material();
            _shaderLambert->setVec3( "u_material.ambient", _materialPtr->ambient );
            _shaderLambert->setVec3( "u_material.diffuse", _materialPtr->diffuse );
            if ( _materialPtr->albedoMap() )
            {
                _shaderLambert->setInt( "u_material.albedoMapActive", 1 );
                _shaderLambert->setInt( "u_material.albedoMap", MESH_RENDERER_ALBEDO_MAP_SLOT );
                glActiveTexture( GL_TEXTURE0 );
                glBindTexture( GL_TEXTURE_2D, _materialPtr->albedoMap()->openglId() );
            }
            else
            {
                _shaderLambert->setInt( "u_material.albedoMapActive", 0 );
            }

            // setup mesh transforms-uniforms
            auto _matModel = meshPtr->matModel();
            _shaderLambert->setMat4( "u_modelMatrix", _matModel );
            _shaderLambert->setMat4( "u_normalMatrix", ( _matModel.inverse() ).transpose() );

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

        // unbind albedo-map and depth-map (end with texture-0 to go back to default-texture 0 
        // being active, in case someone binds this slot without activating it)
        glActiveTexture( GL_TEXTURE2 );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, 0 );

        _shaderLambert->unbind();
    }

    void CMeshRenderer::_render_Phong( const std::vector< CMesh* >& meshes, bool renderWithShadows )
    {
        auto _shaderPhong = CShaderManager::GetCachedShader( "engine_phong_shadows" );

        // setup render state
        _shaderPhong->bind();
        _setupRenderState_camera( _shaderPhong );
        _setupRenderState_light( _shaderPhong );
        if ( renderWithShadows )
        _setupRenderState_shadows( _shaderPhong );
        if ( m_context.fogEnabled == 1 )
            _setupRenderState_fog( _shaderPhong );

        // configure viewer position (required for specular-calculations)
        _shaderPhong->setVec3( "u_viewerPosition", m_context.viewPosition );

        for ( auto meshPtr : meshes )
        {
            // setup material uniforms
            auto _materialPtr = meshPtr->material();
            _shaderPhong->setVec3( "u_material.ambient", _materialPtr->ambient );
            _shaderPhong->setVec3( "u_material.diffuse", _materialPtr->diffuse );
            _shaderPhong->setVec3( "u_material.specular", _materialPtr->specular );
            _shaderPhong->setFloat( "u_material.shininess", _materialPtr->shininess );
            if ( _materialPtr->albedoMap() )
            {
                _shaderPhong->setInt( "u_material.albedoMapActive", 1 );
                _shaderPhong->setInt( "u_material.albedoMap", MESH_RENDERER_ALBEDO_MAP_SLOT );
                glActiveTexture( GL_TEXTURE0 );
                glBindTexture( GL_TEXTURE_2D, _materialPtr->albedoMap()->openglId() );
            }
            else
            {
                _shaderPhong->setInt( "u_material.albedoMapActive", 0 );
            }

            if ( _materialPtr->specularMap() )
            {
                _shaderPhong->setInt( "u_material.specularMapActive", 1 );
                _shaderPhong->setInt( "u_material.specularMap", MESH_RENDERER_SPECULAR_MAP_SLOT );
                glActiveTexture( GL_TEXTURE1 );
                glBindTexture( GL_TEXTURE_2D, _materialPtr->specularMap()->openglId() );
            }
            else
            {
                _shaderPhong->setInt( "u_material.specularMapActive", 0 );
            }

            // setup mesh transforms-uniforms
            auto _matModel = meshPtr->matModel();
            _shaderPhong->setMat4( "u_modelMatrix", _matModel );
            _shaderPhong->setMat4( "u_normalMatrix", ( _matModel.inverse() ).transpose() );

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

        // unbind albedo-map, specular-map and depth-map (end with texture-0 to go back to 
        // default-texture 0 being active, in case someone binds this slot without activating it)
        glActiveTexture( GL_TEXTURE2 );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glActiveTexture( GL_TEXTURE1 );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, 0 );

        _shaderPhong->unbind();
    }

    void CMeshRenderer::_setupRenderState_camera( CShader* shaderPtr )
    {
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
        glActiveTexture( GL_TEXTURE2 );
        glBindTexture( GL_TEXTURE_2D, m_context.shadowMappingTextureId );
    }

    void CMeshRenderer::_setupRenderState_fog( CShader* shaderPtr )
    {
        shaderPtr->setInt( "u_fog.enabled", m_context.fogEnabled );
        shaderPtr->setInt( "u_fog.type", m_context.fogType );
        shaderPtr->setVec3( "u_fog.color", m_context.fogColor );
        shaderPtr->setFloat( "u_fog.density", m_context.fogDensity );
        shaderPtr->setFloat( "u_fog.gradient", m_context.fogGradient );
        shaderPtr->setFloat( "u_fog.distStart", m_context.fogDistStart );
        shaderPtr->setFloat( "u_fog.distEnd", m_context.fogDistEnd );
    }
}