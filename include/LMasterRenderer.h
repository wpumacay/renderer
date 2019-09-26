
#pragma once

#include <lights/CILight.h>
#include <lights/CDirectionalLight.h>
#include <lights/CPointLight.h>
#include <lights/CSpotLight.h>
#include <LShadowMap.h>
#include <LMeshRenderer.h>
#include <LSkyboxRenderer.h>

namespace engine
{

    class LMasterRenderer
    {
    public :
        LMasterRenderer();
        ~LMasterRenderer();

        void render( LScene* pScene );
        void _renderScene( LScene* pScene );
        bool _renderToShadowMap( LScene* pScene );
        void _renderSceneWithShadowMap( LScene* pScene );

        void enableShadows();
        void disableShadows();

        bool areShadowsEnabled() { return m_shadowsEnabled; }

    private :
        // shadow resources
        LShadowMap* m_shadowMap;
        bool m_shadowsEnabled;

        LMeshRenderer* m_meshRenderer;
        LSkyboxRenderer* m_skyboxRenderer;

    };

}