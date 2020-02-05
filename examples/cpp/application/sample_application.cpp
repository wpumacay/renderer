
#include <CEngine.h>

int main()
{
    auto _windowProperties = engine::CWindowProps();
    _windowProperties.width = 1024;
    _windowProperties.height = 768;
    _windowProperties.title = "resizable-application";
    _windowProperties.clearColor = { 0.2f, 0.2f, 0.2f, 1.0f };
    _windowProperties.resizable = true;

    auto _app = std::make_unique<engine::CApplication>( _windowProperties );
    _app->setGuiUtilsActive( true );

    auto _scene = std::make_unique<engine::CScene>();

    /* create some lights for the scene ***********************************************************/
    auto _dirlight = std::make_unique<engine::CDirectionalLight>( "directional",
                                                                  engine::CVec3( 0.4f, 0.4f, 0.4f ),
                                                                  engine::CVec3( 0.8f, 0.8f, 0.8f ),
                                                                  engine::CVec3( 0.8f, 0.8f, 0.8f ),
                                                                  engine::CVec3( -1.0f, -1.0f, -1.0f ) );
    auto _dirlightRef = dynamic_cast<engine::CDirectionalLight*>( _scene->addLight( std::move( _dirlight ) ) );

    auto _pointlight = std::make_unique<engine::CPointLight>( "point",
                                                              engine::CVec3( 0.4f, 0.4f, 0.4f ),
                                                              engine::CVec3( 0.8f, 0.8f, 0.8f ),
                                                              engine::CVec3( 0.8f, 0.8f, 0.8f ),
                                                              engine::CVec3( 5.0f, 5.0f, 5.0f ),
                                                              1.0f, 0.0f, 0.0f );
    auto _pointlightRef = dynamic_cast<engine::CPointLight*>( _scene->addLight( std::move( _pointlight ) ) );
    /* create some cameras for the scene **********************************************************/
    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.aspect = _app->window()->aspect();
    _cameraProjData.width = 10.0f * _app->window()->aspect();
    _cameraProjData.height = 10.0f;

    auto _orbitCamera = std::make_unique<engine::COrbitCamera>( "orbit",
                                                                engine::CVec3( 5.0f, 5.0f, 5.0f ),
                                                                engine::CVec3( 0.0f, 0.0f, 0.0f ),
                                                                engine::eAxis::Z,
                                                                _cameraProjData,
                                                                _app->window()->width(),
                                                                _app->window()->height() );
    auto _orbitCameraRef = _scene->addCamera( std::move( _orbitCamera ) );

    const float _cameraSensitivity  = 0.1f;
    const float _cameraSpeed        = 50.0f;
    const float _cameraMaxDelta     = 10.0f;
    
    auto _fpsCamera = std::make_unique<engine::CFpsCamera>( "fps",
                                                            engine::CVec3( 5.0f, 5.0f, 5.0f ),
                                                            engine::CVec3( 0.0f, 0.0f, 0.0f ),
                                                            engine::eAxis::Z,
                                                            _cameraProjData,
                                                            _cameraSensitivity,
                                                            _cameraSpeed,
                                                            _cameraMaxDelta );
    auto _fpsCameraRef = _scene->addCamera( std::move( _fpsCamera ) );

    auto _fixedCamera = std::make_unique<engine::CFixedCamera>( "fixed",
                                                                engine::CVec3( 5.0f, 5.0f, 5.0f ),
                                                                engine::CVec3( 0.0f, 0.0f, 0.0f ),
                                                                engine::eAxis::Z,
                                                                _cameraProjData );
    auto _fixedCameraRef = _scene->addCamera( std::move( _fixedCamera ) );
    _scene->changeToCamera( _fixedCameraRef->name() );
    /* add some effects like fog and a skybox *****************************************************/

    auto _skybox = std::make_unique<engine::CSkybox>();
    _skybox->setCubemap( engine::CTextureManager::GetCachedTextureCube( "cloudtop" ) );
    auto _skyboxRef = _scene->addSkybox( std::move( _skybox ) );

    /**********************************************************************************************/
    const int _nWidthSamples = 50;
    const int _nDepthSamples = 50;
    const float _widthExtent = 10.0f;
    const float _depthExtent = 10.0f;
    const float _centerX = _widthExtent / 2.0f;
    const float _centerY = _depthExtent / 2.0f;
    std::vector< float > _heightData;
    for ( size_t i = 0; i < _nWidthSamples; i++ )
    {
        for ( size_t j = 0; j < _nDepthSamples; j++ )
        {
            float _x = _widthExtent * ( ( (float) i ) / _nWidthSamples - 0.5f );
            float _y = _depthExtent * ( ( (float) j ) / _nDepthSamples - 0.5f );
            
            // float _z = 10.0f * ( _x * _x + _y * _y ) / ( _widthExtent * _widthExtent + _depthExtent * _depthExtent );

            float _u = _x * 2.0f;
            float _v = _y * 2.0f;
            float _z = std::cos( std::sqrt( ( _u * _u + _v * _v ) ) );
            // float _z = 1.0f;

            _heightData.push_back( _z );
        }
    }

    auto _patch = engine::CMeshBuilder::createHeightField( _nWidthSamples, _nDepthSamples,
                                                           _widthExtent, _depthExtent,
                                                           _centerX, _centerY,
                                                           _heightData, 1.0f,
                                                           engine::eAxis::Z );
    _patch->material()->ambient = { 0.5f, 0.5f, 0.5f };
    _patch->material()->diffuse = { 0.5f, 0.5f, 0.5f };
    _patch->material()->specular = { 0.5f, 0.5f, 0.5f };
    _patch->material()->shininess = 32.0f;

    auto _floor = engine::CMeshBuilder::createPlane( _widthExtent, _depthExtent, engine::eAxis::Z );
    _floor->material()->ambient = { 0.3f, 0.5f, 0.8f };
    _floor->material()->diffuse = { 0.3f, 0.5f, 0.8f };
    _floor->material()->specular = { 0.3f, 0.5f, 0.8f };
    _floor->material()->shininess = 32.0f;

    //// auto _texture = engine::CTextureManager::GetCachedTexture( "img_grid" );
    //// auto _texture = engine::CTextureManager::GetCachedTexture( "img_smiley" );
    auto _texture = engine::CTextureManager::GetCachedTexture( "built_in_chessboard" );
    _patch->material()->setAlbedoMap( _texture );
    _floor->material()->setAlbedoMap( _texture );

    /* create some renderables in our scene *******************************************************/
    std::string _modelpath = std::string( ENGINE_RESOURCES_PATH ) + "models/pokemons/lizardon/lizardon.obj";
    auto _model = engine::CMeshBuilder::createModelFromFile( _modelpath );
    _model->scale = { 0.1f, 0.1f, 0.1f };
    _model->position = { 0.0f, 0.0f, 2.0f };
    _model->rotation = engine::rotationZ( ENGINE_PI / 2.0f ) * engine::rotationX( ENGINE_PI / 2.0f );

    auto _modelRef = _scene->addRenderable( std::move( _model ) );
    auto _patchRef = _scene->addRenderable( std::move( _patch ) );
    auto _floorRef = _scene->addRenderable( std::move( _floor ) );
    /**********************************************************************************************/

    _app->setScene( std::move( _scene ) );

    _app->renderOptions().useShadowMapping = true;
    _app->renderOptions().shadowMapRangeConfig.type = engine::eShadowRangeType::FIXED_USER;
    _app->renderOptions().shadowMapRangeConfig.worldUp = { 0.0f, 1.0f, 0.0f };
    _app->renderOptions().shadowMapRangeConfig.cameraPtr = _orbitCameraRef;
    _app->renderOptions().shadowMapRangeConfig.clipSpaceWidth   = 40.0f;
    _app->renderOptions().shadowMapRangeConfig.clipSpaceHeight  = 40.0f;
    _app->renderOptions().shadowMapRangeConfig.clipSpaceDepth   = 40.0f;
    //// _app->renderOptions().shadowMapRangeConfig.pointLightPtr = _pointlightRef;
    _app->renderOptions().shadowMapRangeConfig.dirLightPtr = _dirlightRef;

    const float rho = 8.660254037844386f; // fixed rho, computed from initial camera position (5,5,5)
    const float phi = 0.6154797086703873f; // fixed phi, computed from initial camera position (5,5,5)
    float theta = 0.7853981633974483f; // initial rho, computed from initial camera position (5,5,5)

    while ( _app->active() )
    {
        if ( engine::CInputManager::IsKeyDown( engine::Keys::KEY_ESCAPE ) )
            break;
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_G ) )
            _app->setGuiActive( !_app->guiActive() );
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_U ) )
            _app->setGuiUtilsActive( !_app->guiUtilsActive() );

        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        theta += 0.2f * engine::CTime::GetAvgTimeStep();
        float _sphi = std::sin( phi );
        float _cphi = std::cos( phi );
        float _stheta = std::sin( theta );
        float _ctheta = std::cos( theta );

        float x = rho * _sphi * _ctheta;
        float y = rho * _sphi * _stheta;
        float z = rho * _cphi;

        _fixedCameraRef->setPosition( { x, y, z } );

        _app->update();

        _app->begin();
        _app->render();
        _app->end();
    }

    return 0;
}