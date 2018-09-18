
#pragma once

#include <LMesh.h>
#include <LICamera.h>
#include <LIRenderer.h>
#include <LShadowMap.h>


namespace engine
{

    class LMeshRenderer : public LIRenderer
    {

        private :

        std::vector< LMesh* > m_renderList;

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