
#include <LMasterRenderer.h>

using namespace std;

namespace engine
{

    LMasterRenderer::LMasterRenderer()
    {
        m_meshRenderer = new LMeshRenderer();
        m_skyboxRenderer = new LSkyboxRenderer();

        m_shadowMap = new LShadowMap();
        m_shadowsEnabled = true;
    }

    LMasterRenderer::~LMasterRenderer()
    {
        if ( m_shadowMap )
            delete m_shadowMap;

        if ( m_meshRenderer )
            delete m_meshRenderer;

        if ( m_skyboxRenderer )
            delete m_skyboxRenderer;
        
        m_shadowMap = NULL;
        m_meshRenderer = NULL;
        m_skyboxRenderer = NULL;
    }

    void LMasterRenderer::render( LScene* pScene )
    {
        // render geometries from the scene
        m_meshRenderer->begin( pScene );

        bool _success = false;

        if ( m_shadowsEnabled )
        {
            auto _dirLights = pScene->getLights< LLightDirectional >();
            if ( _dirLights.size() > 0 )
            {
                _success = _renderToShadowMap( pScene );
                if ( _success )
                {
                    // if could draw to shadowmap, then continue to second pass
                    _renderSceneWithShadowMap( pScene );
                }
                else
                {
                    // if could not draw to shadowmap, then render without shadows
                    cout << "WARNING> Something went wrong while doing shadow mapping first pass" << endl;
                    _renderScene( pScene );
                }
            }
            else
            {
                _renderScene( pScene );
            }
        }
        else
        {
            _renderScene( pScene );
        }

        m_meshRenderer->end( pScene );

        // render the skybox first
        m_skyboxRenderer->begin( pScene );
        m_skyboxRenderer->renderScene( pScene );
        m_skyboxRenderer->end( pScene );
    }

    void LMasterRenderer::_renderScene( LScene* pScene )
    {
        m_meshRenderer->renderScene( pScene );
    }

    bool LMasterRenderer::_renderToShadowMap( LScene* pScene )
    {
        LLightDirectional* _light = NULL;
        // Get directional light
        vector<LLightDirectional*> _lights = pScene->getLights< LLightDirectional >();
        if ( _lights.size() < 1 )
        {
            cout << "ERROR> There is no directional light for shadowmapping" << endl;
            return false;
        }
        _light = _lights[0];

        m_shadowMap->bind();

        m_shadowMap->setupLightDirectional( _light );
        m_meshRenderer->renderToShadowMap( pScene, m_shadowMap );

        m_shadowMap->unbind();

        return true;
    }

    void LMasterRenderer::_renderSceneWithShadowMap( LScene* pScene )
    {
        m_meshRenderer->renderSceneWithShadowMap( pScene, m_shadowMap );
    }

    void LMasterRenderer::enableShadows() 
    { 
        m_shadowsEnabled = true;
    }

    void LMasterRenderer::disableShadows() 
    { 
        m_shadowsEnabled = false;
    }

}