
#include <LMeshRenderer.h>
#include <shaders/LShaderEntitiesLighting.h>
#include <shaders/LShaderShadowMap.h>
#include <shaders/LShaderEntitiesLightingShadows.h>

namespace engine
{

    LMeshRenderer::LMeshRenderer()
    {

    }

    LMeshRenderer::~LMeshRenderer()
    {
        m_renderList.clear();
        m_texturedList.clear();
        m_nonTexturedList.clear();
    }

    void LMeshRenderer::begin( LScene* pScene )
    {
        // Collect mesh-renderables
        auto _renderables = pScene->getRenderables();
        for ( LIRenderable* _renderable : _renderables )
        {
            if ( !_renderable->isVisible() )
            {
                continue;
            }

            if ( _renderable->getType() == RENDERABLE_TYPE_MESH )
            {
                m_renderList.push_back( ( LMesh* )_renderable );

                // add it to the appropiate texture-list
                if ( _renderable->hasTextures() )
                {
                    m_texturedList.push_back( ( LMesh* ) _renderable );
                }
                else
                {
                    m_nonTexturedList.push_back( ( LMesh* ) _renderable );
                }
            }
        }
    }

    void LMeshRenderer::_renderScene( LScene* pScene, bool textured )
    {
        std::string _shaderId = ( textured ? "lighting_entities_textured" : "lighting_entities" );

        // Use simple lighting shader
        auto _shader = ( LShaderEntitiesLighting* ) LShaderManager::getShader( _shaderId );
        // Get current camera for uniforms loading
        auto _camera = pScene->getCurrentCamera();
        if ( _camera == NULL )
        {
            std::cout << "ERROR> There is no current camera in the scene. No render executed" << std::endl;
            return;
        }
        // Get current light for uniforms loading
        auto _lights = pScene->getLights< LLightDirectional >();
        if ( _lights.size() < 1 )
        {
            std::cout << "ERROR> There is no light in the scene. No render executed" << std::endl;
            return;
        }

        auto _light =  _lights[0];

        // start render pass
        _shader->bind();

        // Load uniforms
        _shader->setLightDirectional( _light );
        _shader->setViewMatrix( _camera->getViewMatrix() );
        _shader->setProjectionMatrix( _camera->getProjectionMatrix() );
        _shader->setViewPosition( _camera->getPosition() );
        _shader->setGlobalAmbientLight( LVec3( 0.15f, 0.15f, 0.15f ) );

        // render all meshes
        if ( textured )
        {
            for ( LMesh* _mesh : m_texturedList )
            {
                _shader->setModelMatrix( _mesh->getModelMatrix() );
                _shader->setMaterial( _mesh->getMaterial() );

                _mesh->render();
            }
        }
        else
        {
            for ( LMesh* _mesh : m_nonTexturedList )
            {
                _shader->setModelMatrix( _mesh->getModelMatrix() );
                _shader->setMaterial( _mesh->getMaterial() );

                _mesh->render();
            }
        }

        _shader->unbind();
        // end render pass
    }

    void LMeshRenderer::renderScene( LScene* pScene )
    {
        if ( m_texturedList.size() > 0 )
        {
            // render textured meshes
            _renderScene( pScene, true );
        }
        if ( m_nonTexturedList.size() > 0 )
        {
            // render non-textured meshes
            _renderScene( pScene, false );
        }
    }

    void LMeshRenderer::renderToShadowMap( LScene* pScene, LShadowMap* shadowMap )
    {
        // Use the appropiate shader for the first pass of the shadow mapping procedure
        auto _shader = ( LShaderShadowMap* ) LShaderManager::getShader( "shadow_mapping" );

        _shader->bind();

        _shader->setLightSpaceViewMatrix( shadowMap->getLightSpaceViewMatrix() );
        _shader->setLightSpaceProjectionMatrix( shadowMap->getLightSpaceProjectionMatrix() );

        for ( LMesh* _mesh : m_renderList )
        {
            _shader->setModelMatrix( _mesh->getModelMatrix() );
            _mesh->render();
        }

        _shader->unbind();
    }

    void LMeshRenderer::_renderSceneWithShadowMap( LScene* pScene, LShadowMap* shadowMap, bool textured )
    {
        std::string _shaderId = ( textured ? "lighting_entities_textured_shadows" : "lighting_entities_shadows" );
        auto _shader = ( LShaderEntitiesLightingShadows* ) LShaderManager::getShader( _shaderId );
        // Get current camera for uniforms loading
        auto _camera = pScene->getCurrentCamera();
        if ( _camera == NULL )
        {
            std::cout << "ERROR> There is no current camera in the scene. No render executed" << std::endl;
            return;
        }
        // Get current light for uniforms loading
        auto _lights = pScene->getLights< LLightDirectional >();
        if ( _lights.size() < 1 )
        {
            std::cout << "ERROR> There is no light in the scene. No render executed" << std::endl;
            return;
        }
        auto _light = _lights[0];

        _shader->bind();
        // if textured, the shadowmap is the second texture ( TEXTURE1 )
        if ( textured )
        {
            _shader->setInt( "u_shadowMap", 1 );
            glActiveTexture( GL_TEXTURE0 + 1 );
        }
        glBindTexture( GL_TEXTURE_2D, shadowMap->getDepthTexture() );
        
        _shader->setLightDirectional( _light );
        _shader->setViewMatrix( _camera->getViewMatrix() );
        _shader->setProjectionMatrix( _camera->getProjectionMatrix() );
        _shader->setViewPosition( _camera->getPosition() );
        _shader->setGlobalAmbientLight( LVec3( 0.15f, 0.15f, 0.15f ) );
        _shader->setLightSpaceViewMatrix( shadowMap->getLightSpaceViewMatrix() );
        _shader->setLightSpaceProjectionMatrix( shadowMap->getLightSpaceProjectionMatrix() );
        _shader->setLightPosition( shadowMap->getLightPosition() );

        // render all meshes
        if ( textured )
        {
            for ( LMesh* _mesh : m_texturedList )
            {
                _shader->setModelMatrix( _mesh->getModelMatrix() );
                _shader->setMaterial( _mesh->getMaterial() );

                _mesh->render();
            }
        }
        else
        {
            for ( LMesh* _mesh : m_nonTexturedList )
            {
                _shader->setModelMatrix( _mesh->getModelMatrix() );
                _shader->setMaterial( _mesh->getMaterial() );

                _mesh->render();
            }
        }

        glBindTexture( GL_TEXTURE_2D, 0 );
        _shader->unbind();
    }

    void LMeshRenderer::renderSceneWithShadowMap( LScene* pScene, LShadowMap* shadowMap )
    {
        if ( m_texturedList.size() > 0 )
        {
            // render textured meshes
            _renderSceneWithShadowMap( pScene, shadowMap, true );
        }
        if ( m_nonTexturedList.size() > 0 )
        {
            // render non-textured meshes
            _renderSceneWithShadowMap( pScene, shadowMap, false );
        }
    }

    void LMeshRenderer::end( LScene* pScene )
    {
        m_renderList.clear();
        m_texturedList.clear();
        m_nonTexturedList.clear();
    }
}