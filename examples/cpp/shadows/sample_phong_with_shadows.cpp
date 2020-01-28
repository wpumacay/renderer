
#include <CEngine.h>

void renderToShadowMap( engine::CILight* lightPtr,
                        engine::CShadowMap* shadowMapPtr,
                        engine::CShader* shaderPtr,
                        engine::CIRenderable* floor,
                        std::vector< engine::CIRenderable* > cubes );

void renderSceneWithShadows( engine::CILight* lightPtr, 
                             engine::CICamera* cameraPtr, 
                             engine::CShadowMap* shadowMapPtr,
                             engine::CShader* shaderPtr,
                             engine::CMaterial* floorMaterialPtr,
                             engine::CMaterial* cubeMaterialPtr,
                             engine::CIRenderable* floor,
                             std::vector< engine::CIRenderable* > cubes );

void renderShadowMapVisualization( engine::CVertexArray* quadVAO,
                                   engine::CShader* shaderPtr,
                                   engine::CShadowMap* shadowMapPtr );

engine::CVec3 g_lightPosition = { -2.0f, 4.0f, -1.0f };
// engine::CVec3 g_lightPosition = { 0.0f, 4.0f, 0.0f };

int main()
{
    auto _app = std::make_unique<engine::CApplication>();

    /* load the shader used to render the scene normally (single-light for now) */
    std::string _baseNamePhongWithShadows = std::string( ENGINE_EXAMPLES_PATH ) + "shadows/shaders/phong_with_shadows";
    auto _shaderPhongWithShadowsRef = engine::CShaderManager::CreateShaderFromFiles( "phong_with_shadows_shader",
                                                                                     _baseNamePhongWithShadows + "_vs.glsl",
                                                                                     _baseNamePhongWithShadows + "_fs.glsl" );
    ENGINE_ASSERT( _shaderPhongWithShadowsRef, "Could not load phong-with-shadows shader to render the scene using the shadowmap T_T" );

    /* load the shader used for shadow mapping */
    std::string _baseNameShadowMapProjection = std::string( ENGINE_EXAMPLES_PATH ) + "shadows/shaders/shadowmap_projection";
    auto _shaderShadowMapProjRef = engine::CShaderManager::CreateShaderFromFiles( "shadowmap_projection_shader",
                                                                                  _baseNameShadowMapProjection + "_vs.glsl",
                                                                                  _baseNameShadowMapProjection + "_fs.glsl" );
    ENGINE_ASSERT( _shaderShadowMapProjRef, "Couldn't load shadow-mapping shader use to create the depth-map T_T" );

    /* load the shader in charge of depth-map visualization */
    std::string _baseNameShadowMapViz = std::string( ENGINE_EXAMPLES_PATH ) + "shadows/shaders/shadowmap_visualization";
    auto _shaderShadowMapVizRef = engine::CShaderManager::CreateShaderFromFiles( "shadowmap_visualization_shader",
                                                                                 _baseNameShadowMapViz + "_vs.glsl",
                                                                                 _baseNameShadowMapViz + "_fs.glsl" );
    ENGINE_ASSERT( _shaderShadowMapVizRef, "Couldn't load the visualization shader to check the depth-map T_T" );

    /* Create a simple scene for testing **********************************************************/

    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = _app->window()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 100.0f;

    //// auto _camera = std::make_unique<engine::COrbitCamera>( "orbit",
    ////                                                        engine::CVec3( 0.0f, 0.0f, 3.0f ),
    ////                                                        engine::CVec3( 0.0f, 0.0f, 0.0f ),
    ////                                                        engine::eAxis::Y,
    ////                                                        _cameraProjData,
    ////                                                        _app->window()->width(),
    ////                                                        _app->window()->height() );

    const float _cameraSensitivity  = 0.25f;
    const float _cameraSpeed        = 25.0f;
    const float _cameraMaxDelta     = 10.0f;
    
    auto _camera = std::make_unique<engine::CFpsCamera>( "fps",
                                                         engine::CVec3( 2.0f, 2.0f, 2.0f ),
                                                         engine::CVec3( 0.0f, 0.0f, 0.0f ),
                                                         engine::eAxis::Y,
                                                         _cameraProjData,
                                                         _cameraSensitivity,
                                                         _cameraSpeed,
                                                         _cameraMaxDelta );

    auto _cameraRef = _app->scene()->addCamera( std::move( _camera ) );

    auto _floor = engine::CMeshBuilder::createPlane( 50.0f, 50.0f, engine::eAxis::Y );
    _floor->position = { 0.0f, 0.0f, 0.0f };

    auto _cube1 = engine::CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
    _cube1->position = { 0.0f, 2.0f, 0.0f };

    auto _cube2 = engine::CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
    _cube2->position = { 2.0f, 0.5f, 1.0f };

    auto _cube3 = engine::CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
    _cube3->position = { -1.0f, 0.5f, 2.0f };
    _cube3->rotation = tinymath::rotation( engine::CVec3( 1.0f, 0.0f, 1.0f ), engine::toRadians( 60.0f ) );
    _cube3->scale = { 0.5f, 0.5f, 0.5f };

    auto _floorTextureRef = engine::CTextureManager::GetCachedTexture( "img_wooden_floor" );
    auto _cubeTextureRef = engine::CTextureManager::GetCachedTexture( "img_wooden_floor" );

    ENGINE_ASSERT( _floorTextureRef, "Could not retrieve valid texture for the sample - floor" );
    ENGINE_ASSERT( _cubeTextureRef, "Could not retrieve valid texture for the sample - cube" );

    auto _floorMaterial = std::make_unique<engine::CMaterial>( "floor_material",
                                                               engine::eMaterialType::PHONG,
                                                               engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                               engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                               engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                               64.0f,
                                                               _floorTextureRef,
                                                               _floorTextureRef );

    auto _cubeMaterial = std::make_unique<engine::CMaterial>( "cube_material",
                                                              engine::eMaterialType::PHONG,
                                                              engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                              engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                              engine::CVec3( 1.0f, 1.0f, 1.0f ),
                                                              64.0f,
                                                              _floorTextureRef,
                                                              _floorTextureRef );

    auto _dirlight = std::make_unique<engine::CDirectionalLight>( "directional",
                                                                  engine::CVec3( 0.3f, 0.3f, 0.3f ),
                                                                  engine::CVec3( 0.3f, 0.3f, 0.3f ),
                                                                  engine::CVec3( 0.3f, 0.3f, 0.3f ),
                                                                  ( engine::CVec3( 0.0f, 0.0f, 0.0f ) - g_lightPosition ).normalized() );

    auto _pointlight = std::make_unique<engine::CPointLight>( "point",
                                                              engine::CVec3( 0.3f, 0.3f, 0.3f ),
                                                              engine::CVec3( 0.3f, 0.3f, 0.3f ),
                                                              engine::CVec3( 0.3f, 0.3f, 0.3f ),
                                                              g_lightPosition,
                                                              1.0f, 0.09f, 0.032f );

    /**********************************************************************************************/

    auto _currentLightRef = _dirlight.get();
    auto _shadowmap = std::make_unique<engine::CShadowMap>( 2048, 2048 );

    engine::float32 _quad_buffData[] = {
     /*|  positions |     uvs    |*/
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f
    };

    engine::uint32 _quad_indices[] = {
        0, 1, 2, 
        0, 2, 3
    };

    engine::CVertexBufferLayout _layout = { { "pos", engine::eElementType::Float2, false },
                                            { "uv", engine::eElementType::Float2, false } };

    auto _quad_vbuffer = std::make_unique<engine::CVertexBuffer>( _layout, engine::eBufferUsage::STATIC,
                                                                  sizeof( _quad_buffData ), _quad_buffData );

    auto _quad_ibuffer = std::make_unique<engine::CIndexBuffer>( engine::eBufferUsage::STATIC, 6, _quad_indices );

    auto _quad_varray = std::make_unique<engine::CVertexArray>();
    _quad_varray->addVertexBuffer( std::move( _quad_vbuffer ) );
    _quad_varray->setIndexBuffer( std::move( _quad_ibuffer ) );

    while( _app->active() )
    {
        if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_ESCAPE ) )
            break;
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_SPACE ) )
            _cameraRef->setActiveMode( false );
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_ENTER ) )
            _cameraRef->setActiveMode( true );

        if ( _cameraRef->type() == engine::CFpsCamera::GetStaticType() )
        {
            if ( _cameraRef->active() )
                _app->window()->disableCursor();
            else
                _app->window()->enableCursor();
        }

        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        _app->update();
        _app->begin();
        _cameraRef->update();

        /* do our thing here ************************/

        // render to shadow map first
        renderToShadowMap( _currentLightRef, _shadowmap.get(), _shaderShadowMapProjRef, _floor.get(), { _cube1.get(), _cube2.get(), _cube3.get() } );

        // render the scene normally
        renderSceneWithShadows( _currentLightRef, _cameraRef, _shadowmap.get(), _shaderPhongWithShadowsRef, _floorMaterial.get(), _cubeMaterial.get(), _floor.get(), { _cube1.get(), _cube2.get(), _cube3.get() } );

        // render the shadowmap to a quad
        renderShadowMapVisualization( _quad_varray.get(), _shaderShadowMapVizRef, _shadowmap.get() );

        /********************************************/

        _app->render();
        _app->end();
    }

    return 0;
}

void renderToShadowMap( engine::CILight* lightPtr,
                        engine::CShadowMap* shadowMapPtr,
                        engine::CShader* shaderPtr,
                        engine::CIRenderable* floor,
                        std::vector< engine::CIRenderable* > cubes )
{
    // glCullFace( GL_FRONT );
    shaderPtr->bind();
    shadowMapPtr->bind();

    const float _znear = 1.0f;
    const float _zfar = 7.5f;
    auto _lightViewMat = engine::lookAt( g_lightPosition, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
    auto _lightProjMat = engine::ortho( 20, 20, _znear, _zfar );

    shaderPtr->setMat4( "u_lightSpaceViewProjMatrix", _lightProjMat * _lightViewMat );

    {
        shaderPtr->setMat4( "u_modelMatrix", floor->matModel() );
        floor->render();
    }

    for ( size_t i = 0; i < cubes.size(); i++ )
    {
        shaderPtr->setMat4( "u_modelMatrix", cubes[i]->matModel() );
        cubes[i]->render();
    }

    shadowMapPtr->unbind();
    shaderPtr->unbind();
    // glCullFace( GL_BACK );
}

void renderSceneWithShadows( engine::CILight* lightPtr, 
                             engine::CICamera* cameraPtr, 
                             engine::CShadowMap* shadowMapPtr,
                             engine::CShader* shaderPtr,
                             engine::CMaterial* floorMaterialPtr,
                             engine::CMaterial* cubeMaterialPtr,
                             engine::CIRenderable* floor,
                             std::vector< engine::CIRenderable* > cubes )
{
    if ( !lightPtr || !cameraPtr || !shaderPtr )
        return;

    shaderPtr->bind();
    shaderPtr->setInt( "u_spotLight.enabled", 0 );
    shaderPtr->setInt( "u_pointLight.enabled", 0 );
    shaderPtr->setInt( "u_directionalLight.enabled", 0 );

    /* setup the light according to its type */
    if ( lightPtr->type() == engine::eLightType::DIRECTIONAL )
    {
        shaderPtr->setInt( "u_directionalLight.enabled", 1 );
        shaderPtr->setVec3( "u_directionalLight.ambient", lightPtr->ambient );
        shaderPtr->setVec3( "u_directionalLight.diffuse", lightPtr->diffuse );
        shaderPtr->setVec3( "u_directionalLight.specular", lightPtr->specular );
        shaderPtr->setFloat( "u_directionalLight.intensity", lightPtr->intensity );
        shaderPtr->setVec3( "u_directionalLight.direction", lightPtr->direction );
    }
    else if ( lightPtr->type() == engine::eLightType::POINT )
    {
        shaderPtr->setInt( "u_pointLight.enabled", 1 );
        shaderPtr->setVec3( "u_pointLight.ambient", lightPtr->ambient );
        shaderPtr->setVec3( "u_pointLight.diffuse", lightPtr->diffuse );
        shaderPtr->setVec3( "u_pointLight.specular", lightPtr->specular );
        shaderPtr->setFloat( "u_pointLight.intensity", lightPtr->intensity );
        shaderPtr->setVec3( "u_pointLight.position", lightPtr->position );
        shaderPtr->setFloat( "u_pointLight.attnk0", lightPtr->atnConstant );
        shaderPtr->setFloat( "u_pointLight.attnk1", lightPtr->atnLinear );
        shaderPtr->setFloat( "u_pointLight.attnk2", lightPtr->atnQuadratic );
    }
    else if ( lightPtr->type() == engine::eLightType::SPOT )
    {
        shaderPtr->setInt( "u_spotLight.enabled", 1 );
        shaderPtr->setVec3( "u_spotLight.ambient", lightPtr->ambient );
        shaderPtr->setVec3( "u_spotLight.diffuse", lightPtr->diffuse );
        shaderPtr->setVec3( "u_spotLight.specular", lightPtr->specular );
        shaderPtr->setFloat( "u_spotLight.intensity", lightPtr->intensity );
        shaderPtr->setVec3( "u_spotLight.position", lightPtr->position );
        shaderPtr->setFloat( "u_spotLight.attnk0", lightPtr->atnConstant );
        shaderPtr->setFloat( "u_spotLight.attnk1", lightPtr->atnLinear );
        shaderPtr->setFloat( "u_spotLight.attnk2", lightPtr->atnQuadratic );
        shaderPtr->setVec3( "u_spotLight.direction", lightPtr->direction );
        shaderPtr->setFloat( "u_spotLight.innerCutoffCos", std::cos( lightPtr->innerCutoff ) );
        shaderPtr->setFloat( "u_spotLight.outerCutoffCos", std::cos( lightPtr->outerCutoff ) );
    }

    /* setup the view and proj matrices */
    shaderPtr->setMat4( "u_viewProjMatrix", cameraPtr->matProj() * cameraPtr->matView() );
    shaderPtr->setVec3( "u_viewerPosition", cameraPtr->position() );

    /* setup the light-clip-space transform */
    const float _znear = 1.0f;
    const float _zfar = 7.5f;
    auto _lightViewMat = engine::lookAt( g_lightPosition, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
    auto _lightProjMat = engine::ortho( 20, 20, _znear, _zfar );
    shaderPtr->setMat4( "u_viewProjLightSpaceMatrix", _lightProjMat * _lightViewMat );

    /* configure the texture unit for our shadowmap's depth texture (slot 3 in the shader) */
    shaderPtr->setInt( "u_depthmapTexture", 3 );
    glActiveTexture( GL_TEXTURE3 );
    shadowMapPtr->frameBuffer()->getTextureAttachment( "shadow_depth_attachment" )->bind();

    /* render the floor */
    floorMaterialPtr->bind( shaderPtr );
    {
        auto _modelMat = floor->matModel();
        shaderPtr->setMat4( "u_modelMatrix", _modelMat );
        shaderPtr->setMat4( "u_normalMatrix", _modelMat.inverse().transpose() );
        floor->render();
    }
    floorMaterialPtr->unbind();

    /* render the cubes (all use the same material) */
    cubeMaterialPtr->bind( shaderPtr );
    for ( size_t i = 0; i < cubes.size(); i++ )
    {
        auto _modelMat = cubes[i]->matModel();
        shaderPtr->setMat4( "u_modelMatrix", _modelMat );
        shaderPtr->setMat4( "u_normalMatrix", _modelMat.inverse().transpose() );
        cubes[i]->render();
    }
    cubeMaterialPtr->unbind();

    shadowMapPtr->frameBuffer()->getTextureAttachment( "shadow_depth_attachment" )->bind();
    shaderPtr->unbind();
}

void renderShadowMapVisualization( engine::CVertexArray* quadVAO,
                                   engine::CShader* shaderPtr,
                                   engine::CShadowMap* shadowMapPtr )
{
    glDisable( GL_DEPTH_TEST );
    glViewport( 0, 0, 256, 256 );
    shaderPtr->bind();
    shadowMapPtr->frameBuffer()->getTextureAttachment( "shadow_depth_attachment" )->bind();
    quadVAO->bind();

    glDrawElements( GL_TRIANGLES, quadVAO->indexBuffer()->count(), GL_UNSIGNED_INT, 0 );

    quadVAO->unbind();
    shadowMapPtr->frameBuffer()->getTextureAttachment( "shadow_depth_attachment" )->unbind();
    shaderPtr->unbind();
    glEnable( GL_DEPTH_TEST );
    glViewport( 0, 0, engine::CApplication::GetInstance()->window()->width(), engine::CApplication::GetInstance()->window()->height() );
}
