
#pragma once

#include <LMesh.h>
#include <LIRenderer.h>
#include <LShadowMap.h>


namespace engine
{

    class LMeshRenderer : public LIRenderer
    {

        private :

        std::vector< LMesh* > m_renderList;
        std::vector< LMesh* > m_texturedList;
        std::vector< LMesh* > m_nonTexturedList;

        // void _renderSceneTexturedMeshes( LScene* pScene );
        // void _renderSceneNonTexturedMeshes( LScene* pScene );

        // void _renderSceneWithShadowMapTexturedMeshes( LScene* pScene );
        // void _renderSceneWithShadowMapNonTexturedMeshes( LScene* pScene );

        void _renderScene( LScene* pScene, bool textured );
        void _renderSceneWithShadowMap( LScene* pScene, LShadowMap* shadowMap, bool textured );

        public :

        LMeshRenderer();
        ~LMeshRenderer();

        void begin( LScene* pScene ) override;
        void renderScene( LScene* pScene ) override;
        void renderToShadowMap( LScene* pScene, LShadowMap* shadowMap ) override;
        void renderSceneWithShadowMap( LScene* pScene, LShadowMap* shadowMap ) override;
        void end( LScene* pScene ) override;
    };



}