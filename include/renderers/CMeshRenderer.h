#pragma once

#include <renderers/CRenderersCommon.h>

namespace engine
{

    const int MESH_RENDERER_ALBEDO_MAP_SLOT = 0;
    const int MESH_RENDERER_SPECULAR_MAP_SLOT = 1;
    const int MESH_RENDERER_NORMAL_MAP_SLOT = 2;
    const int MESH_RENDERER_DEPTH_MAP_SLOT = 3;

    struct CMeshComparatorFarthestFromPoint
    {
        CVec3 point;

        bool operator() ( CMesh* mesh1, CMesh* mesh2 )
        {
            return ( mesh1->position - point ).length() > ( mesh2->position - point ).length();
        }
    };

    struct CMeshComparatorClosesToPoint
    {
        CVec3 point;

        bool operator() ( CMesh* mesh1, CMesh* mesh2 )
        {
            return ( mesh1->position - point ).length() < ( mesh2->position - point ).length();
        }
    };

    struct CMeshRenderContext
    {
        /* some render options */
        bool    useFog;
        bool    useFaceCulling;
        bool    useBlending;
        bool    useShadowMapping;
        /* camera information */
        CMat4   viewMatrix;
        CMat4   projMatrix;
        CVec3   viewPosition;
        /* light information */
        eLightType  lightType;
        CVec3       lightAmbient;
        CVec3       lightDiffuse;
        CVec3       lightSpecular;
        float32     lightIntensity;
        CVec3       lightPosition;
        CVec3       lightDirection;
        float32     lightInnerCutoffCos;
        float32     lightOuterCutoffCos;
        float32     lightAttnK0;
        float32     lightAttnK1;
        float32     lightAttnK2;
        /* when using shadow mapping */
        CMat4   shadowMappingViewMatrix;
        CMat4   shadowMappingProjMatrix;
        int32   shadowMappingPCFcount;
        int32   shadowMappingSize;
        uint32  shadowMappingTextureId;
        /* when using fog */
        int32   fogType;
        CVec3   fogColor;
        float32 fogDensity;
        float32 fogGradient;
        float32 fogDistStart;
        float32 fogDistEnd;
        /* when rendering in depth-only mode */
        float32 depthViewZnear; // from camera frustum
        float32 depthViewZfar; // from camera frustum
        float32 depthViewZmin;
        float32 depthViewZmax;
        CVec3   depthViewZminColor;
        CVec3   depthViewZmaxColor;
        /* when rendering in semantic-only mode */
        std::unordered_map< int32, CVec3 > semanticViewIdMap;
    };

    class CMeshRenderer
    {

    public :

        CMeshRenderer();
        ~CMeshRenderer();

        void begin( const CRenderOptions& renderOptions );
        void submit( const std::vector< CMesh* >& meshesVisible,
                     const std::vector< CMesh* >& meshesInView );

        void renderToShadowMap();
        void renderMeshesOpaque();
        void renderMeshesTransparent();
        void renderDepthOnly();
        void renderSemanticOnly();

        std::string status() const { return m_status; }

    private :

        void _setupRenderState_camera( CShader* shaderPtr );
        void _setupRenderState_light( CShader* shaderPtr );
        void _setupRenderState_shadows( CShader* shaderPtr );
        void _setupRenderState_fog( CShader* shaderPtr );

        void _renderMeshes( CShader* shaderPtr,
                            const std::vector< CMesh* >& meshesWithFaceCulling,
                            const std::vector< CMesh* >& meshesWitNoFaceCulling,
                            bool renderWithBlending );

        void _renderMesh( CShader* shaderPtr, CMesh* meshPtr );

    private :

        std::string m_status;
        CMeshRenderContext m_context;

        std::vector< CMesh* > m_meshesCastShadows;
        std::vector< CMesh* > m_meshesDontCastShadows;

        std::vector< CMesh* > m_meshesOpaque;
        std::vector< CMesh* > m_meshesTransparent;

        std::default_random_engine m_randGen;
        std::uniform_real_distribution< float32 > m_randDist;

        std::unordered_map< int32, CVec3 > m_cachedRandomColors;

        CShader* m_shaderDepthView;
        CShader* m_shaderSemanticView;
        CShader* m_shaderShadowMapping;

        CShader* m_shaderNoShadowsNoFog;
        CShader* m_shaderNoShadowsFog;
        CShader* m_shaderShadowsNoFog;
        CShader* m_shaderShadowsFog;
    };

}