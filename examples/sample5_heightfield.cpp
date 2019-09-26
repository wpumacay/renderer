
#include <CEngine.h>

int main()
{
    auto _app = new engine::COpenGLApp();
    _app->init();

    auto _scene = _app->scene();

    // make a sample mesh just for testing
    std::string _modelpath;
    _modelpath += ENGINE_RESOURCES_PATH;
    _modelpath += "models/pokemons/lizardon.obj";

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

            _heightData.push_back( _z );
        }
    }

    auto _patch = engine::CMeshBuilder::createHeightField( _nWidthSamples, _nDepthSamples,
                                                           _widthExtent, _depthExtent,
                                                           _centerX, _centerY,
                                                           _heightData, 1.0f );
    _patch->getMaterial()->setColor( { 0.5f, 0.5f, 0.5f } );

    auto _model = engine::CMeshBuilder::createModelFromFile( _modelpath, "lizardon" );
    _model->scale = { 0.05f, 0.05f, 0.05f };
    _model->pos = { 0.0f, 0.0f, 3.0f };

    // make a sample camera
    auto _cameraProjData = engine::CCameraProjData();

    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = engine::COpenGLApp::GetWindow()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 100.0f;

//     const float _cameraSensitivity  = 0.25f;
//     const float _cameraSpeed        = 5.0f;
//     const float _cameraMaxDelta     = 10.0f;
// 
//     auto _camera = new engine::CFpsCamera( "fps",
//                                            engine::CVec3( 2.0f, 2.0f, 2.0f ),
//                                            engine::CVec3( 0.0f, 0.0f, 0.0f ),
//                                            engine::eAxis::Z,
//                                            _cameraProjData,
//                                            _cameraSensitivity,
//                                            _cameraSpeed,
//                                            _cameraMaxDelta );

//     auto _camera = new engine::CFixedCamera( "fixed",
//                                              engine::CVec3( 1.0f, 2.0f, 1.0f ),
//                                              engine::CVec3( 0.0f, 0.0f, 0.0f ),
//                                              engine::eAxis::Z,
//                                              _cameraProjData );

    auto _camera = new engine::COrbitCamera( "orbit",
                                             engine::CVec3( 2.0f, 2.0f, 2.0f ),
                                             engine::CVec3( 0.0f, 0.0f, 0.0f ),
                                             engine::eAxis::Z,
                                             _cameraProjData,
                                             engine::COpenGLApp::GetWindow()->width(),
                                             engine::COpenGLApp::GetWindow()->height() );

    _camera->setPosition( { -1.77f, -4.12f, 4.37f } );

    // make a sample light source
    auto _light = new engine::CDirectionalLight( "directional_1",
                                                 { 0.2f, 0.2f, 0.2f }, 
                                                 { 0.5f, 0.5f, 0.5f },
                                                 { 0.05f, 0.05f, 0.05f }, 
                                                 { 0.0f, 0.0f, -1.0f } );

    // make a simple skybox
    auto _skybox = new engine::LSkybox( "starfield" );
    // add these components to the scene
    _scene->addCamera( _camera );
    _scene->addLight( _light );
    _scene->addRenderable( _model );
    _scene->addRenderable( _patch );
    _scene->addSkybox( _skybox );

    while( _app->isActive() )
    {
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        _model->rotation = _model->rotation * engine::CMat4::rotationX( 0.01f );

        if ( engine::CInputHandler::IsKeyDown( ENGINE_KEY_SPACE ) )
            _scene->getCurrentCamera()->setActiveMode( false );
        else if ( engine::CInputHandler::IsKeyDown( ENGINE_KEY_ENTER ) )
            _scene->getCurrentCamera()->setActiveMode( true );
        else if ( engine::CInputHandler::IsKeyDown( ENGINE_KEY_L ) )
            _patch->setWireframeMode( true );
        else if ( engine::CInputHandler::IsKeyDown( ENGINE_KEY_N ) )
            _patch->setWireframeMode( false );
        else if ( engine::CInputHandler::IsKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        _app->begin();
        _app->renderScene();
        _app->renderUi();
        _app->end();
    }

    return 0;
}
