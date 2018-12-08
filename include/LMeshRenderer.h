
#pragma once

#include <LIRenderable.h>
#include <LMesh.h>
#include <LModel.h>
#include <LIRenderer.h>
#include <LShadowMap.h>


namespace engine
{

    class LMeshRenderer : public LIRenderer
    {

        private :

        std::vector< LIRenderable* > m_renderList;
        std::vector< LIRenderable* > m_texturedList;
        std::vector< LIRenderable* > m_nonTexturedList;
        std::vector< LIRenderable* > m_debugList;

        // void _renderSceneTexturedMeshes( LScene* pScene );
        // void _renderSceneNonTexturedMeshes( LScene* pScene );

        // void _renderSceneWithShadowMapTexturedMeshes( LScene* pScene );
        // void _renderSceneWithShadowMapNonTexturedMeshes( LScene* pScene );

        void _renderDebug( LScene* pScene );

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