
#include <CEngine.h>

class ApplicationUi : public engine::CImguiUi
{

public :

    ApplicationUi( engine::COpenGLContext* context ) 
        : engine::CImguiUi( context ) {}

    ~ApplicationUi() {}

protected :

    void _initInternal() override
    {
        // nothing for now
    }

    void _renderInternal() override
    {
        // nothing for now
    }
};

void renderToShadowMap( engine::CILight* lightPtr,
                        engine::CShadowMap* shadowMapPtr,
                        engine::CShader* shaderPtr,
                        engine::LIRenderable* floor,
                        std::vector< engine::LIRenderable* > cubes );

void renderSceneWithShadows( engine::CILight* lightPtr, 
                             engine::CICamera* cameraPtr, 
                             engine::CShadowMap* shadowMapPtr,
                             engine::CShader* shaderPtr,
                             engine::CPhongMaterial* floorMaterialPtr,
                             engine::CPhongMaterial* cubeMaterialPtr,
                             engine::LIRenderable* floor,
                             std::vector< engine::LIRenderable* > cubes );

void renderShadowMapVisualization( engine::CVertexArray* quadVAO,
                                   engine::CShader* shaderPtr,
                                   engine::CShadowMap* shadowMapPtr );

struct ComparatorDotDirection
{
    engine::CVec3 direction;

    bool operator() ( engine::CVec3 v1, engine::CVec3 v2 )
    {
        auto _dot1 = engine::CVec3::dot( direction, v1 );
        auto _dot2 = engine::CVec3::dot( direction, v2 );

        return _dot1 < _dot2;
    }
};

void showDirectionalLightVolume( engine::CICamera* cameraPtr, const engine::CVec3& direction )
{
    if ( !cameraPtr )
        return;

    auto _viewProjMatrix = cameraPtr->matProj() * cameraPtr->matView();

    engine::CDebugDrawer::DrawClipVolume( _viewProjMatrix, { 1.0f, 1.0f, 0.0f } );

    /* get back the corners of the frustum in world space */
    engine::CMat4 _invClipMatrix = _viewProjMatrix.inverse();

    engine::CVec3 _frustumPointsClipSpace[8] = {
        /*      near plane      */
        { -1.0f, -1.0f, -1.0f }, 
        { 1.0f, -1.0f, -1.0f },
        { 1.0f,  1.0f, -1.0f },
        { -1.0f,  1.0f, -1.0f },
        /*      far plane       */
        { -1.0f, -1.0f, 1.0f }, 
        { 1.0f, -1.0f, 1.0f },
        { 1.0f,  1.0f, 1.0f },
        { -1.0f,  1.0f, 1.0f }
    };


    std::vector< engine::CVec3 > _points3d;
    for ( size_t q = 0; q < 8; q++ )
    {
        engine::CVec4 _pointFrustum = _invClipMatrix * engine::CVec4( _frustumPointsClipSpace[q], 1.0f );
        engine::CVec3 _pointFrustumNormalized = { _pointFrustum.x / _pointFrustum.w,
                                                  _pointFrustum.y / _pointFrustum.w,
                                                  _pointFrustum.z / _pointFrustum.w };

        _points3d.push_back( _pointFrustumNormalized );
    }


    /* construct a frame using the direction vector as front */
    engine::CVec3 _fvec, _rvec, _uvec;
    engine::CVec3 _worldUp = { 0.0f, 1.0f, 0.0f };

    if ( engine::CVec3::equal( direction, { 0.0f, 1.0f, 0.0f } ) )
    {
        _fvec = _worldUp;
        _rvec = { _worldUp.z, _worldUp.x, _worldUp.y };
        _uvec = { _worldUp.y, _worldUp.z, _worldUp.x };
    }
    else if ( engine::CVec3::equal( direction + _worldUp, { 0.0f, 0.0f, 0.0f } ) )
    {
        _fvec = -_worldUp;
        _rvec = { _worldUp.z, _worldUp.x, _worldUp.y };
        _uvec = { _worldUp.y, _worldUp.z, _worldUp.x };
    }
    else
    {
        _fvec = direction;
        _rvec = engine::CVec3::cross( { 0.0f, 1.0f, 0.0f }, _fvec );
        _uvec = engine::CVec3::cross( _fvec, _rvec );
    }

    _fvec.normalize();
    _rvec.normalize();
    _uvec.normalize();

    // std::cout << "***********************************************************" << std::endl;
    // std::cout << "direction: " << engine::toString( direction ) << std::endl;
    // std::cout << "fvec: " << engine::toString( _fvec ) << std::endl;
    // std::cout << "rvec: " << engine::toString( _rvec ) << std::endl;
    // std::cout << "uvec: " << engine::toString( _uvec ) << std::endl;

    /* sort over f-vector */
    auto _fPoints3d = _points3d; // create a copy
    {
        auto _fComparator = ComparatorDotDirection();
        _fComparator.direction = _fvec;

        std::sort( _fPoints3d.begin(), _fPoints3d.end(), _fComparator );
    }
    /* sort over r-vector */
    auto _rPoints3d = _points3d; // create a copy
    {
        auto _rComparator = ComparatorDotDirection();
        _rComparator.direction = _rvec;

        std::sort( _rPoints3d.begin(), _rPoints3d.end(), _rComparator );
    }
    /* sort over u-vector */
    auto _uPoints3d = _points3d; // create a copy
    {
        auto _uComparator = ComparatorDotDirection();
        _uComparator.direction = _uvec;

        std::sort( _uPoints3d.begin(), _uPoints3d.end(), _uComparator );
    }

    float _df = std::abs( engine::CVec3::dot( _fPoints3d.back() - _fPoints3d.front(), _fvec ) );
    float _dr = std::abs( engine::CVec3::dot( _rPoints3d.back() - _rPoints3d.front(), _rvec ) );
    float _du = std::abs( engine::CVec3::dot( _uPoints3d.back() - _uPoints3d.front(), _uvec ) );

    auto _center = engine::CVec3::dot( 0.5f * ( _fPoints3d.front() + _fPoints3d.back() ), _fvec ) * _fvec +
                   engine::CVec3::dot( 0.5f * ( _rPoints3d.front() + _rPoints3d.back() ), _rvec ) * _rvec +
                   engine::CVec3::dot( 0.5f * ( _uPoints3d.front() + _uPoints3d.back() ), _uvec ) * _uvec;

    engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, _fPoints3d.front(), { 0.5f, 0.0f, 0.0f } );
    engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, _fPoints3d.back(), { 1.0f, 0.0f, 0.0f } );

    engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, _rPoints3d.front(), { 0.0f, 0.5f, 0.0f } );
    engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, _rPoints3d.back(), { 0.0f, 1.0f, 0.0f } );

    engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, _uPoints3d.front(), { 0.0f, 0.0f, 0.5f } );
    engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, _uPoints3d.back(), { 0.0f, 0.0f, 1.0f } );

    engine::CDebugDrawer::DrawArrow( { 3.0f, 3.0f, 3.0f }, engine::CVec3( 3.0f, 3.0f, 3.0f ) + direction, { 0.0f, 1.0f, 1.0f } );
    engine::CDebugDrawer::DrawAxes( _fvec, _rvec, _uvec, _center, 0.5f );

    // define the points of the oobb
    auto _p0 = _center - ( 0.5f * _df ) * _fvec - ( 0.5f * _dr ) * _rvec - ( 0.5f * _du ) * _uvec;
    auto _p1 = _center - ( 0.5f * _df ) * _fvec - ( 0.5f * _dr ) * _rvec + ( 0.5f * _du ) * _uvec;
    auto _p2 = _center - ( 0.5f * _df ) * _fvec + ( 0.5f * _dr ) * _rvec - ( 0.5f * _du ) * _uvec;
    auto _p3 = _center - ( 0.5f * _df ) * _fvec + ( 0.5f * _dr ) * _rvec + ( 0.5f * _du ) * _uvec;
    auto _p4 = _center + ( 0.5f * _df ) * _fvec - ( 0.5f * _dr ) * _rvec - ( 0.5f * _du ) * _uvec;
    auto _p5 = _center + ( 0.5f * _df ) * _fvec - ( 0.5f * _dr ) * _rvec + ( 0.5f * _du ) * _uvec;
    auto _p6 = _center + ( 0.5f * _df ) * _fvec + ( 0.5f * _dr ) * _rvec - ( 0.5f * _du ) * _uvec;
    auto _p7 = _center + ( 0.5f * _df ) * _fvec + ( 0.5f * _dr ) * _rvec + ( 0.5f * _du ) * _uvec;

    engine::CVec3 _bcolor = { 0.7f, 0.5f, 0.3f };

    engine::CDebugDrawer::DrawLine( _p0, _p1, _bcolor ); engine::CDebugDrawer::DrawLine( _p2, _p3, _bcolor );
    engine::CDebugDrawer::DrawLine( _p0, _p2, _bcolor ); engine::CDebugDrawer::DrawLine( _p1, _p3, _bcolor );

    engine::CDebugDrawer::DrawLine( _p4, _p5, _bcolor ); engine::CDebugDrawer::DrawLine( _p6, _p7, _bcolor );
    engine::CDebugDrawer::DrawLine( _p4, _p6, _bcolor ); engine::CDebugDrawer::DrawLine( _p5, _p7, _bcolor );

    engine::CDebugDrawer::DrawLine( _p2, _p6, _bcolor ); engine::CDebugDrawer::DrawLine( _p3, _p7, _bcolor );
    engine::CDebugDrawer::DrawLine( _p0, _p4, _bcolor ); engine::CDebugDrawer::DrawLine( _p1, _p5, _bcolor );

//     auto _comparator = ComparatorDotDirection();
//     _comparator.direction = direction;
// 
//     // sort the points of the frustum according to the dot-product to the direction
//     std::sort( _points3d.begin(), _points3d.end(), _comparator );
// 
//     for ( size_t i = 0; i < _points3d.size(); i++ )
//     {
//         auto _dot = engine::CVec3::dot( _points3d[i], direction );
//         std::cout << "sort-dot(" << i << "): " << _dot << std::endl;;
//         std::cout << "sort-vec(" << i << "): " << engine::toString( _points3d[i] ) << std::endl;
//     }
// 
//     engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, _points3d.front(), { 1.0f, 1.0f, 0.5f } );
//     engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, _points3d.back(), { 1.0f, 0.5f, 1.0f } );
}

engine::CVec3 g_lightPosition = { -2.0f, 4.0f, -1.0f };
// engine::CVec3 g_lightPosition = { 0.0f, 4.0f, 0.0f };

int main()
{
    auto _app = new engine::COpenGLApp();
    _app->init();

    auto _ui = new ApplicationUi( engine::COpenGLApp::GetWindow()->context() );
    _ui->init();

    _app->setUi( _ui );

    /* load the shader used to render the scene normally (single-light for now) */
    std::string _baseNamePhongWithShadows = std::string( ENGINE_EXAMPLES_PATH ) + "shadows/shaders/phong_with_shadows";
    auto _shaderPhongWithShadows = engine::CShaderManager::CreateShaderFromFiles( "phong_with_shadows_shader",
                                                                                  _baseNamePhongWithShadows + "_vs.glsl",
                                                                                  _baseNamePhongWithShadows + "_fs.glsl" );
    ENGINE_ASSERT( _shaderPhongWithShadows, "Could not load phong-with-shadows shader to render the scene using the shadowmap T_T" );

    /* load the shader used for shadow mapping */
    std::string _baseNameShadowMapProjection = std::string( ENGINE_EXAMPLES_PATH ) + "shadows/shaders/shadowmap_projection";
    auto _shaderShadowMapProj = engine::CShaderManager::CreateShaderFromFiles( "shadowmap_projection_shader",
                                                                               _baseNameShadowMapProjection + "_vs.glsl",
                                                                               _baseNameShadowMapProjection + "_fs.glsl" );
    ENGINE_ASSERT( _shaderShadowMapProj, "Couldn't load shadow-mapping shader use to create the depth-map T_T" );

    /* load the shader in charge of depth-map visualization */
    std::string _baseNameShadowMapViz = std::string( ENGINE_EXAMPLES_PATH ) + "shadows/shaders/shadowmap_visualization";
    auto _shaderShadowMapViz = engine::CShaderManager::CreateShaderFromFiles( "shadowmap_visualization_shader",
                                                                             _baseNameShadowMapViz + "_vs.glsl",
                                                                             _baseNameShadowMapViz + "_fs.glsl" );
    ENGINE_ASSERT( _shaderShadowMapViz, "Couldn't load the visualization shader to check the depth-map T_T" );

    /* Create a simple scene for testing **********************************************************/

    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = engine::COpenGLApp::GetWindow()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 100.0f;

    // auto _camera = new engine::COrbitCamera( "orbit",
    //                                          { 0.0f, 0.0f, 3.0f },
    //                                          { 0.0f, 0.0f, 0.0f },
    //                                          engine::eAxis::Y,
    //                                          _cameraProjData,
    //                                          engine::COpenGLApp::GetWindow()->width(),
    //                                          engine::COpenGLApp::GetWindow()->height() );

    const float _cameraSensitivity  = 0.25f;
    const float _cameraSpeed        = 250.0f;
    const float _cameraMaxDelta     = 10.0f;
    
    auto _camera = new engine::CFpsCamera( "fps",
                                           { 2.0f, 2.0f, 2.0f },
                                           { 0.0f, 0.0f, 0.0f },
                                           engine::eAxis::Y,
                                           _cameraProjData,
                                           _cameraSensitivity,
                                           _cameraSpeed,
                                           _cameraMaxDelta );

    /* create a dummy camera to visualize the clipping volume */
    auto _cameraProjDataTest = engine::CCameraProjData();
    _cameraProjDataTest.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjDataTest.fov         = 45.0f;
    _cameraProjDataTest.aspect      = engine::COpenGLApp::GetWindow()->aspect();
    _cameraProjDataTest.zNear       = 1.0f;
    _cameraProjDataTest.zFar        = 4.0f;

    auto _cameraTest = new engine::CFixedCamera( "fixed",
                                                  engine::CVec3( 5.0f, 5.0f, 5.0f ),
                                                  engine::CVec3( 0.0f, 0.0f, 0.0f ),
                                                  engine::eAxis::Z,
                                                  _cameraProjDataTest );

    auto _floor = engine::CMeshBuilder::createPlane( 50.0f, 50.0f, engine::eAxis::Y );
    _floor->pos = { 0.0f, 0.0f, 0.0f };

    auto _cube1 = engine::CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
    _cube1->pos = { 0.0f, 2.0f, 0.0f };

    auto _cube2 = engine::CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
    _cube2->pos = { 2.0f, 0.5f, 1.0f };

    auto _cube3 = engine::CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
    _cube3->pos = { -1.0f, 0.5f, 2.0f };
    _cube3->rotation = engine::CMat4::rotation( engine::toRadians( 60.0f ), { 1.0f, 0.0f, 1.0f } );
    _cube3->scale = { 0.5f, 0.5f, 0.5f };

    auto _floorTexture = engine::CTextureManager::GetCachedTexture( "img_wooden_floor" );
    auto _cubeTexture = engine::CTextureManager::GetCachedTexture( "img_wooden_floor" );

    ENGINE_ASSERT( _floorTexture, "Could not retrieve valid texture for the sample - floor" );
    ENGINE_ASSERT( _cubeTexture, "Could not retrieve valid texture for the sample - cube" );

    auto _floorMaterial = new engine::CPhongMaterial( "floor_material",
                                                      { 1.0f, 1.0f, 1.0f },
                                                      { 1.0f, 1.0f, 1.0f },
                                                      { 1.0f, 1.0f, 1.0f },
                                                      64.0f,
                                                      _floorTexture,
                                                      _floorTexture );

    auto _cubeMaterial = new engine::CPhongMaterial( "cube_material",
                                                     { 1.0f, 1.0f, 1.0f },
                                                     { 1.0f, 1.0f, 1.0f },
                                                     { 1.0f, 1.0f, 1.0f },
                                                     64.0f,
                                                     _floorTexture,
                                                     _floorTexture );

    auto _dirlight = new engine::CDirectionalLight( "directional",
                                                    { 0.3f, 0.3f, 0.3f },
                                                    { 0.3f, 0.3f, 0.3f },
                                                    { 0.3f, 0.3f, 0.3f },
                                                    engine::CVec3::normalize( engine::CVec3( 0.0f, 0.0f, 0.0f ) - g_lightPosition ) );

    auto _pointlight = new engine::CPointLight( "point",
                                                { 0.3f, 0.3f, 0.3f },
                                                { 0.3f, 0.3f, 0.3f },
                                                { 0.3f, 0.3f, 0.3f },
                                                g_lightPosition,
                                                1.0f, 0.09f, 0.032f );

    /**********************************************************************************************/

    auto _currentLight = _dirlight;
    auto _shadowmap = new engine::CShadowMap( 2048, 2048 );

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

    auto _quad_vbuffer = new engine::CVertexBuffer( _layout,
                                                    engine::eBufferUsage::STATIC,
                                                    sizeof( _quad_buffData ),
                                                    _quad_buffData );;

    auto _quad_ibuffer = new engine::CIndexBuffer( engine::eBufferUsage::STATIC,
                                                   6, _quad_indices );

    auto _quad_varray = new engine::CVertexArray();
    _quad_varray->addVertexBuffer( _quad_vbuffer );
    _quad_varray->setIndexBuffer( _quad_ibuffer );

    while( _app->isActive() )
    {
        if ( engine::CInputHandler::CheckSingleKeyPress( ENGINE_KEY_ESCAPE ) )
            break;
        else if ( engine::CInputHandler::CheckSingleKeyPress( ENGINE_KEY_SPACE ) )
            _camera->setActiveMode( false );
        else if ( engine::CInputHandler::CheckSingleKeyPress( ENGINE_KEY_ENTER ) )
            _camera->setActiveMode( true );

        if ( _camera->type() == engine::CFpsCamera::GetStaticType() )
        {
            if ( _camera->active() )
                engine::COpenGLApp::GetWindow()->disableCursor();
            else
                engine::COpenGLApp::GetWindow()->enableCursor();
        }

        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        showDirectionalLightVolume( _cameraTest, engine::CVec3::normalize( engine::CVec3( 0.0f, 0.0f, 0.0f ) - g_lightPosition ) );
        // showDirectionalLightVolume( _cameraTest, { 0.0f, -1.0f, 0.0f } );

        _app->begin();
        _camera->update();

        /* do our thing here ************************/

        // render to shadow map first
        renderToShadowMap( _currentLight, _shadowmap, _shaderShadowMapProj.get(), _floor, { _cube1, _cube2, _cube3 } );

        // render the scene normally
        renderSceneWithShadows( _currentLight, _camera, _shadowmap, _shaderPhongWithShadows.get(), _floorMaterial, _cubeMaterial, _floor, { _cube1, _cube2, _cube3 } );

        // render the shadowmap to a quad
        renderShadowMapVisualization( _quad_varray, _shaderShadowMapViz.get(), _shadowmap );

        /********************************************/

        engine::CDebugDrawer::Render( _camera );

        if ( !_camera->active() )
            _app->renderUi();

        _app->end();
    }

    return 0;
}

void renderToShadowMap( engine::CILight* lightPtr,
                        engine::CShadowMap* shadowMapPtr,
                        engine::CShader* shaderPtr,
                        engine::LIRenderable* floor,
                        std::vector< engine::LIRenderable* > cubes )
{
    // glCullFace( GL_FRONT );
    shaderPtr->bind();
    shadowMapPtr->bind();

    const float _znear = 1.0f;
    const float _zfar = 7.5f;
    auto _lightViewMat = engine::CMat4::lookAt( g_lightPosition, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
    auto _lightProjMat = engine::CMat4::ortho( 20, 20, _znear, _zfar );

    shaderPtr->setMat4( "u_lightSpaceViewProjMatrix", _lightProjMat * _lightViewMat );

    {
        shaderPtr->setMat4( "u_modelMatrix", floor->getModelMatrix() );
        floor->render();
    }

    for ( size_t i = 0; i < cubes.size(); i++ )
    {
        shaderPtr->setMat4( "u_modelMatrix", cubes[i]->getModelMatrix() );
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
                             engine::CPhongMaterial* floorMaterialPtr,
                             engine::CPhongMaterial* cubeMaterialPtr,
                             engine::LIRenderable* floor,
                             std::vector< engine::LIRenderable* > cubes )
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
        shaderPtr->setVec3( "u_directionalLight.direction", reinterpret_cast< engine::CDirectionalLight* >( lightPtr )->direction );
    }
    else if ( lightPtr->type() == engine::eLightType::POINT )
    {
        shaderPtr->setInt( "u_pointLight.enabled", 1 );
        shaderPtr->setVec3( "u_pointLight.ambient", lightPtr->ambient );
        shaderPtr->setVec3( "u_pointLight.diffuse", lightPtr->diffuse );
        shaderPtr->setVec3( "u_pointLight.specular", lightPtr->specular );
        shaderPtr->setFloat( "u_pointLight.intensity", lightPtr->intensity );
        shaderPtr->setVec3( "u_pointLight.position", reinterpret_cast< engine::CPointLight* >( lightPtr )->position );
        shaderPtr->setFloat( "u_pointLight.attnk0", reinterpret_cast< engine::CPointLight* >( lightPtr )->atnConstant );
        shaderPtr->setFloat( "u_pointLight.attnk1", reinterpret_cast< engine::CPointLight* >( lightPtr )->atnLinear );
        shaderPtr->setFloat( "u_pointLight.attnk2", reinterpret_cast< engine::CPointLight* >( lightPtr )->atnQuadratic );
    }
    else if ( lightPtr->type() == engine::eLightType::SPOT )
    {
        shaderPtr->setInt( "u_spotLight.enabled", 1 );
        shaderPtr->setVec3( "u_spotLight.ambient", lightPtr->ambient );
        shaderPtr->setVec3( "u_spotLight.diffuse", lightPtr->diffuse );
        shaderPtr->setVec3( "u_spotLight.specular", lightPtr->specular );
        shaderPtr->setFloat( "u_spotLight.intensity", lightPtr->intensity );
        shaderPtr->setVec3( "u_spotLight.position", reinterpret_cast< engine::CSpotLight* >( lightPtr )->position );
        shaderPtr->setFloat( "u_spotLight.attnk0", reinterpret_cast< engine::CSpotLight* >( lightPtr )->atnConstant );
        shaderPtr->setFloat( "u_spotLight.attnk1", reinterpret_cast< engine::CSpotLight* >( lightPtr )->atnLinear );
        shaderPtr->setFloat( "u_spotLight.attnk2", reinterpret_cast< engine::CSpotLight* >( lightPtr )->atnQuadratic );
        shaderPtr->setVec3( "u_spotLight.direction", reinterpret_cast< engine::CSpotLight* >( lightPtr )->direction );
        shaderPtr->setFloat( "u_spotLight.innerCutoffCos", std::cos( reinterpret_cast< engine::CSpotLight* >( lightPtr )->innerCutoff ) );
        shaderPtr->setFloat( "u_spotLight.outerCutoffCos", std::cos( reinterpret_cast< engine::CSpotLight* >( lightPtr )->outerCutoff ) );
    }

    /* setup the view and proj matrices */
    shaderPtr->setMat4( "u_viewProjMatrix", cameraPtr->matProj() * cameraPtr->matView() );
    shaderPtr->setVec3( "u_viewerPosition", cameraPtr->position() );

    /* setup the light-clip-space transform */
    const float _znear = 1.0f;
    const float _zfar = 7.5f;
    auto _lightViewMat = engine::CMat4::lookAt( g_lightPosition, { 0.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
    auto _lightProjMat = engine::CMat4::ortho( 20, 20, _znear, _zfar );
    shaderPtr->setMat4( "u_viewProjLightSpaceMatrix", _lightProjMat * _lightViewMat );

    /* configure the texture unit for our shadowmap's depth texture (slot 3 in the shader) */
    shaderPtr->setInt( "u_depthmapTexture", 3 );
    glActiveTexture( GL_TEXTURE3 );
    shadowMapPtr->frameBuffer()->getTextureAttachment( "shadow_depth_attachment" )->bind();

    /* render the floor */
    floorMaterialPtr->bind( shaderPtr );
    {
        auto _modelMat = floor->getModelMatrix();
        shaderPtr->setMat4( "u_modelMatrix", _modelMat );
        shaderPtr->setMat4( "u_normalMatrix", _modelMat.inverse().transpose() );
        floor->render();
    }
    floorMaterialPtr->unbind();

    /* render the cubes (all use the same material) */
    cubeMaterialPtr->bind( shaderPtr );
    for ( size_t i = 0; i < cubes.size(); i++ )
    {
        auto _modelMat = cubes[i]->getModelMatrix();
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
    glViewport( 0, 0, engine::COpenGLApp::GetWindow()->width(), engine::COpenGLApp::GetWindow()->height() );
}
