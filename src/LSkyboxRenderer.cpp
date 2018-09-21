
#include <LSkyboxRenderer.h>
#include <shaders/LShaderSkybox.h>

namespace engine
{

    LSkyboxRenderer::LSkyboxRenderer()
    {
        // Do nothing
    }

    LSkyboxRenderer::~LSkyboxRenderer()
    {
        // Do nothing
    }

    void LSkyboxRenderer::begin( LScene* pScene )
    {
        // glDepthMask( GL_FALSE );
        glDepthFunc( GL_LEQUAL );
    }

    void LSkyboxRenderer::renderScene( LScene* pScene )
    {
        // use skybox shader
        auto _shader = ( LShaderSkybox* ) LShaderManager::getShader( "skybox" );
        // get current camera for uniforms loading
        auto _camera = pScene->getCurrentCamera();
        if ( _camera == NULL )
        {
            std::cout << "ERROR> There is no current camera in the scene. No render executed" << std::endl;
            return;
        }
        // get skybox
        auto _skybox = pScene->getSkybox();
        if ( _skybox != NULL )
        {
            _shader->bind();
            _shader->setViewMatrix( _camera->getViewMatrix() );
            _shader->setProjectionMatrix( _camera->getProjectionMatrix() );

            _skybox->render();

            _shader->unbind();
        }
    }

    void LSkyboxRenderer::renderToShadowMap( LScene* pScene, LShadowMap* shadowMap )
    {
        // Do nothing
    }

    void LSkyboxRenderer::renderSceneWithShadowMap( LScene* pScene, LShadowMap* shadowMap )
    {
        // Do nothing
    }

    void LSkyboxRenderer::end( LScene* pScene )
    {
        // glDepthMask( GL_TRUE );
        glDepthFunc( GL_LESS );
    }


}