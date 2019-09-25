
#include <LMeshRenderer.h>

// @TODO: Modify the rendering process of models, as the texture check is done via the parent, not children

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

            if ( _renderable->debug )
            {
                m_debugList.push_back( _renderable );
            }
            else
            {
                // if ( _renderable->getType() == eRenderableType::MESH )
                // {
                    m_renderList.push_back( _renderable );

                    // add it to the appropiate texture-list
                    if ( _renderable->texture() )
                    {
                        m_texturedList.push_back( _renderable );
                    }
                    else
                    {
                        m_nonTexturedList.push_back( _renderable );
                    }

                    // std::cout << "renderlist.size: " << m_renderList.size() << std::endl;
                // }
            }
        }
    }

    void LMeshRenderer::_renderDebug( LScene* pScene )
    {
        auto _shader = CShaderManager::GetCachedShader( "basic3d_no_textures" );

        auto _camera = pScene->getCurrentCamera();
        if ( _camera == NULL )
        {
            std::cout << "ERROR> There is no current camera in the scene. No render executed" << std::endl;
            return;
        }

        _shader->bind();

        _shader->setMat4( "u_tView", _camera->matView() );
        _shader->setMat4( "u_tProj", _camera->matProj() );

        for ( auto _mesh : m_debugList )
        {
            _shader->setMat4( "u_tModel", _mesh->getModelMatrix() );
            _shader->setVec3( "u_color", _mesh->getMaterial()->diffuse );

            _mesh->render();
        }

        _shader->unbind();
    }

    void LMeshRenderer::_renderScene( LScene* pScene, bool textured )
    {
        std::string _shaderId = ( textured ) ? "phong_textures_no_shadows" : "phong_no_textures_no_shadows";

        // Use simple lighting shader
        auto _shader = CShaderManager::GetCachedShader( _shaderId );
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

        auto _light =  _lights.front();

        // start render pass
        _shader->bind();

        // set light-related uniforms
        _shader->setVec3( "u_directionalLight.ambient", _light->ambient );
        _shader->setVec3( "u_directionalLight.diffuse", _light->diffuse );
        _shader->setVec3( "u_directionalLight.specular", _light->specular );
        _shader->setVec3( "u_directionalLight.direction", _light->direction );
        _shader->setInt( "u_directionalLight.isActive", _light->isActive );

        // set scene related uniforms
        _shader->setMat4( "u_tView", _camera->matView() );
        _shader->setMat4( "u_tProj", _camera->matProj() );
        _shader->setVec3( "u_viewPos", _camera->position() );
        _shader->setVec3( "u_globalAmbientLight", { 0.15f, 0.15f, 0.15f } );

        // render all meshes
        if ( textured )
        {
            for ( auto _mesh : m_texturedList )
            {
                if ( _mesh->getType() == eRenderableType::MODEL )
                {
                    auto _children = reinterpret_cast< LModel* >( _mesh )->getMeshes();
                    //
                    for ( auto _child : _children )
                    {
                        _shader->setMat4( "u_tModel", _mesh->getModelMatrix() );
                        // set material related uniforms
                        _shader->setVec3( "u_material.ambient", _child->getMaterial()->ambient );
                        _shader->setVec3( "u_material.diffuse", _child->getMaterial()->diffuse );
                        _shader->setVec3( "u_material.specular", _child->getMaterial()->specular );
                        _shader->setFloat( "u_material.shininess", _child->getMaterial()->shininess );

                        _child->render();
                    }
                }
                else
                {
                    _shader->setMat4( "u_tModel", _mesh->getModelMatrix() );
                    // set material related uniforms
                    _shader->setVec3( "u_material.ambient", _mesh->getMaterial()->ambient );
                    _shader->setVec3( "u_material.diffuse", _mesh->getMaterial()->diffuse );
                    _shader->setVec3( "u_material.specular", _mesh->getMaterial()->specular );
                    _shader->setFloat( "u_material.shininess", _mesh->getMaterial()->shininess );

                    _mesh->render();
                }
            }
        }
        else
        {
            for ( auto _mesh : m_nonTexturedList )
            {
                if ( _mesh->getType() == eRenderableType::MODEL )
                {
                    auto _children = reinterpret_cast< LModel* >( _mesh )->getMeshes();
                    //
                    for ( auto _child : _children )
                    {
                        _shader->setMat4( "u_tModel", _mesh->getModelMatrix() );
                        // set material related uniforms
                        _shader->setVec3( "u_material.ambient", _child->getMaterial()->ambient );
                        _shader->setVec3( "u_material.diffuse", _child->getMaterial()->diffuse );
                        _shader->setVec3( "u_material.specular", _child->getMaterial()->specular );
                        _shader->setFloat( "u_material.shininess", _child->getMaterial()->shininess );

                        _child->render();
                    }
                }
                else
                {
                    _shader->setMat4( "u_tModel", _mesh->getModelMatrix() );
                    // set material related uniforms
                    _shader->setVec3( "u_material.ambient", _mesh->getMaterial()->ambient );
                    _shader->setVec3( "u_material.diffuse", _mesh->getMaterial()->diffuse );
                    _shader->setVec3( "u_material.specular", _mesh->getMaterial()->specular );
                    _shader->setFloat( "u_material.shininess", _mesh->getMaterial()->shininess );

                    _mesh->render();
                }
            }
        }

        _shader->unbind();
        // end render pass
    }

    void LMeshRenderer::renderScene( LScene* pScene )
    {
        if ( m_debugList.size() > 0 )
        {
            // render debug meshes
            _renderDebug( pScene );
        }

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
        auto _shader = CShaderManager::GetCachedShader( "shadow_mapping" );

        _shader->bind();

        _shader->setMat4( "u_lightSpaceViewMatrix", shadowMap->getLightSpaceViewMatrix() );
        _shader->setMat4( "u_lightSpaceProjMatrix", shadowMap->getLightSpaceProjectionMatrix() );

        for ( auto _mesh : m_renderList )
        {
            _shader->setMat4( "u_modelMatrix", _mesh->getModelMatrix() );
            _mesh->render();
        }

        _shader->unbind();
    }

    void LMeshRenderer::_renderSceneWithShadowMap( LScene* pScene, LShadowMap* shadowMap, bool textured )
    {
        std::string _shaderId = ( textured ) ? "phong_textures_shadows" : "phong_no_textures_shadows";
        auto _shader = CShaderManager::GetCachedShader( _shaderId );
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
        
        // set light-related uniforms
        _shader->setVec3( "u_directionalLight.ambient", _light->ambient );
        _shader->setVec3( "u_directionalLight.diffuse", _light->diffuse );
        _shader->setVec3( "u_directionalLight.specular", _light->specular );
        _shader->setVec3( "u_directionalLight.direction", _light->direction );
        _shader->setInt( "u_directionalLight.isActive", _light->isActive );

        _shader->setMat4( "u_tView", _camera->matView() );
        _shader->setMat4( "u_tProj", _camera->matProj() );
        _shader->setVec3( "u_viewPos", _camera->position() );
        _shader->setVec3( "u_globalAmbientLight", { 0.15f, 0.15f, 0.15f } );
        _shader->setMat4( "u_tLightSpaceViewMatrix", shadowMap->getLightSpaceViewMatrix() );
        _shader->setMat4( "u_tLightSpaceProjMatrix", shadowMap->getLightSpaceProjectionMatrix() );
        // _shader->setVec3( "u_lightPos", shadowMap->getLightPosition() );

        // render all meshes
        if ( textured )
        {
            for ( auto _mesh : m_texturedList )
            {
                if ( _mesh->getType() == eRenderableType::MODEL )
                {
                    auto _children = reinterpret_cast< LModel* >( _mesh )->getMeshes();
                    //
                    for ( auto _child : _children )
                    {
                        _shader->setMat4( "u_tModel", _mesh->getModelMatrix() );
                        // set material related uniforms
                        _shader->setVec3( "u_material.ambient", _child->getMaterial()->ambient );
                        _shader->setVec3( "u_material.diffuse", _child->getMaterial()->diffuse );
                        _shader->setVec3( "u_material.specular", _child->getMaterial()->specular );
                        _shader->setFloat( "u_material.shininess", _child->getMaterial()->shininess );

                        _child->render();
                    }
                }
                else
                {
                    _shader->setMat4( "u_tModel", _mesh->getModelMatrix() );
                    _shader->setVec3( "u_material.ambient", _mesh->getMaterial()->ambient );
                    _shader->setVec3( "u_material.diffuse", _mesh->getMaterial()->diffuse );
                    _shader->setVec3( "u_material.specular", _mesh->getMaterial()->specular );
                    _shader->setFloat( "u_material.shininess", _mesh->getMaterial()->shininess );

                    _mesh->render();
                }
            }
        }
        else
        {
            for ( auto _mesh : m_nonTexturedList )
            {
                if ( _mesh->getType() == eRenderableType::MODEL )
                {
                    auto _children = reinterpret_cast< LModel* >( _mesh )->getMeshes();
                    //
                    for ( auto _child : _children )
                    {
                        _shader->setMat4( "u_tModel", _mesh->getModelMatrix() );
                        // set material related uniforms
                        _shader->setVec3( "u_material.ambient", _child->getMaterial()->ambient );
                        _shader->setVec3( "u_material.diffuse", _child->getMaterial()->diffuse );
                        _shader->setVec3( "u_material.specular", _child->getMaterial()->specular );
                        _shader->setFloat( "u_material.shininess", _child->getMaterial()->shininess );

                        _child->render();
                    }
                }
                else
                {
                    _shader->setMat4( "u_tModel", _mesh->getModelMatrix() );
                    // set material related uniforms
                    _shader->setVec3( "u_material.ambient", _mesh->getMaterial()->ambient );
                    _shader->setVec3( "u_material.diffuse", _mesh->getMaterial()->diffuse );
                    _shader->setVec3( "u_material.specular", _mesh->getMaterial()->specular );
                    _shader->setFloat( "u_material.shininess", _mesh->getMaterial()->shininess );

                    _mesh->render();
                }
            }
        }

        glBindTexture( GL_TEXTURE_2D, 0 );
        _shader->unbind();
    }

    void LMeshRenderer::renderSceneWithShadowMap( LScene* pScene, LShadowMap* shadowMap )
    {
        if ( m_debugList.size() > 0 )
        {
            // render debug meshes
            _renderDebug( pScene );
        }
        
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
        m_debugList.clear();
    }
}