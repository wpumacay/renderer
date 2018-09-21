
#pragma once

#include <LSkybox.h>
#include <LIRenderer.h>

namespace engine
{

    class LSkyboxRenderer : public LIRenderer
    {

        public :

        LSkyboxRenderer();
        ~LSkyboxRenderer();

        void begin( LScene* pScene ) override;
        void renderScene( LScene* pScene ) override;
        void renderToShadowMap( LScene* pScene, LShadowMap* shadowMap ) override;
        void renderSceneWithShadowMap( LScene* pScene, LShadowMap* shadowMap ) override;
        void end( LScene* pScene ) override;
    };



}