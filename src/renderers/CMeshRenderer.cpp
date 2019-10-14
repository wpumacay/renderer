
#include <renderers/CMeshRenderer.h>

namespace engine
{

    CMeshRenderer::CMeshRenderer()
    {

    }

    CMeshRenderer::~CMeshRenderer()
    {

    }

    void CMeshRenderer::submit( const std::vector< CMesh* >& meshes, const CRenderOptions& renderOptions )
    {
        ENGINE_CORE_ASSERT( renderOptions.cameraPtr, "Mesh-renderer requires a valid camera to render" );
        ENGINE_CORE_ASSERT( renderOptions.lightPtr, "Mesh-renderer requires a valid light to render" );

        // (0): clear buffers to store newly submitted meshes, and store render options
        m_meshesCastShadows.clear();
        m_meshesDontCastShadows.clear();
        m_meshesOpaque.clear();
        m_meshesTransparent.clear();

        // (1): separate the ones that cast shadows from the ones that don't (for shadow-map pass)
        for ( auto meshPtr : meshes )
        {
            if ( meshPtr->castsShadows )
                m_meshesCastShadows.push_back( meshPtr );
            else 
                m_meshesDontCastShadows.push_back( meshPtr );
        }

        // (2): separate the ones that are opaque from the ones that are transparent (for blending)
        for ( auto meshPtr : meshes )
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
        m_context.viewMatrix                = renderOptions.cameraPtr->matView();
        m_context.projMatrix                = renderOptions.cameraPtr->matProj();
        m_context.lightType                 = renderOptions.lightPtr->type();
        m_context.lightAmbient              = renderOptions.lightPtr->ambient;
        m_context.lightDiffuse              = renderOptions.lightPtr->diffuse;
        m_context.lightSpecular             = renderOptions.lightPtr->specular;
    }

    void CMeshRenderer::renderToShadowMap()
    {
////         auto _shaderShadowmap = CShaderManager::GetCachedShader( "engine_shadow_mapping" );
////         auto _shadowMap = m_renderOptions.shadowMapPtr;
//// 
////         _shadowMap->bind();
////         _shadowMap->setup();
////         _shaderShadowmap->bind();
////         _shaderShadowmap->setMat4( "u_lightSpaceViewProjMatrix",  )
////         for ( auto meshPtr : m_meshesCastShadows )
////         {
////             m_shaderShadowMapping->setMat4( "u_modelMatrix", meshPtr->matModel() );
////             
////             meshPtr->vertexArray()->bind();
//// 
////             glDrawElements( GL_TRIANGLES,
////                             _mesh->vertexArray()->indexBuffer()->count(),
////                             GL_UNSIGNED_INT, 0 );
//// 
////             meshPtr->vertexArray()->unbind();
////         }
////         _shadowMap->unbind();
////         _shaderShadowmap->unbind();
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
    }

}