#pragma once

#include <renderers/CRenderersCommon.h>

namespace engine
{

    struct CMeshComparatorFarthestFromPoint
    {
        CVec3 point;

        bool operator() ( CMesh* mesh1, CMesh* mesh2 )
        {
            return CVec3::length( mesh1->position - point ) > CVec3::length( mesh2->position - point );
        }
    };

    struct CMeshRenderContext
    {
        /* camera information */
        CMat4   viewMatrix;
        CMat4   projMatrix;
        /* light information */
        eLightType lightType;
        CVec3   lightAmbient;
        CVec3   lightDiffuse;
        CVec3   lightSpecular;
        float32 lightIntensity;
        CVec3   lightPosition;
        CVec3   lightDirection;
        CVec3   lightInnerCutoffCos;
        CVec3   lightOuterCutoffCos;
        CVec3   lightAttnK0;
        CVec3   lightAttnK1;
        CVec3   lightAttnK2;
        /* when using shadow mapping */
        CMat4   shadowMappingViewMatrix;
        CMat4   shadowMappingProjMatrix;
        int32   shadowMappingPCFcount;
        int32   shadowMappingSize;
        uint32  shadowMappingTextureId;
    };

    class CMeshRenderer
    {

    public :

        CMeshRenderer();
        ~CMeshRenderer();

        void submit( const std::vector< CMesh* >& meshes, 
                     const CRenderOptions& renderOptions );

        void renderToShadowMap();
        void renderWithShadowMap();
        void renderWithoutShadowMap();

        template< class T >
        void _collectMeshesByMaterial( const std::vector< CMesh* >& meshes,
                                       std::vector< CMesh* >& meshesOfGivenMaterial )
        {
            for ( auto _mesh : meshes )
                if ( _mesh->material()->type() == T::GetStaticType() )
                    meshesOfGivenMaterial.push_back( _mesh );
        }

        std::string status() const { return m_status; }

    private :

        std::string m_status;
        CMeshRenderContext m_context;

        std::vector< CMesh* > m_meshesCastShadows;
        std::vector< CMesh* > m_meshesDontCastShadows;

        std::vector< CMesh* > m_meshesOpaque;
        std::vector< CMesh* > m_meshesTransparent;
    };

}