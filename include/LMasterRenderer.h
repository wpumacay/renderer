
#pragma once

#include <LILight.h>
#include <LLightDirectional.h>
#include <LShadowMap.h>
#include <LMeshRenderer.h>
#include <LSkyboxRenderer.h>

namespace engine
{


    class LMasterRenderer
    {

        private :

        // shadow resources
        LShadowMap* m_shadowMap;
        bool m_shadowsEnabled;

        LMeshRenderer* m_meshRenderer;
        LSkyboxRenderer* m_skyboxRenderer;

        static LMasterRenderer* _INSTANCE;
        LMasterRenderer();

        public :

        static LMasterRenderer* GetInstance();
        ~LMasterRenderer();

        void render( LScene* pScene );
        void _renderScene( LScene* pScene );
        bool _renderToShadowMap( LScene* pScene );
        void _renderSceneWithShadowMap( LScene* pScene );

        void enableShadows();
        void disableShadows();

        bool areShadowsEnabled() { return m_shadowsEnabled; }

    };



}