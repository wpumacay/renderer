
#include <CEngine.h>

int main()
{
    auto _app = new engine::COpenGLApp();
    _app->init();
    
    auto _scene = _app->scene();
    
    // make a sample mesh just for testing
    std::string _modelpath;
    _modelpath += ENGINE_RESOURCES_PATH;
    _modelpath += "models/chassis.stl";

    auto _model = engine::CMeshBuilder::createModelFromFile( _modelpath, "chassis" );

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

    // auto _camera = new engine::CFixedCamera( "fixed",
    //                                          engine::CVec3( 1.0f, 2.0f, 1.0f ),
    //                                          engine::CVec3( 0.0f, 0.0f, 0.0f ),
    //                                          engine::eAxis::Z,
    //                                          _cameraProjData );

    auto _camera = new engine::COrbitCamera( "orbit",
                                             engine::CVec3( 2.0f, 2.0f, 2.0f ),
                                             engine::CVec3( 0.0f, 0.0f, 0.0f ),
                                             engine::eAxis::Z,
                                             _cameraProjData,
                                             engine::COpenGLApp::GetWindow()->width(),
                                             engine::COpenGLApp::GetWindow()->height() );

    // make a sample light source
    auto _light = new engine::CDirectionalLight( "directional_1",
                                                 { 0.4f, 0.4f, 0.4f }, 
                                                 { 0.8f, 0.8f, 0.8f },
                                                 { 0.3f, 0.3f, 0.3f }, 
                                                 { 0.0f, 0.0f, -1.0f } );

    // add these components to the scene
    _scene->addCamera( _camera );
    _scene->addLight( _light );
    _scene->addRenderable( _model );

    while( _app->active() )
    {
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        _model->rotation = _model->rotation * engine::CMat4::rotationX( 0.01f );

        if ( engine::CInputHandler::IsKeyDown( ENGINE_KEY_SPACE ) )
            _scene->getCurrentCamera()->setActiveMode( false );
        else if ( engine::CInputHandler::IsKeyDown( ENGINE_KEY_ENTER ) )
            _scene->getCurrentCamera()->setActiveMode( true );
        else if ( engine::CInputHandler::IsKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        _app->beginRendering();
        _app->renderScene();
        _app->renderUi();
        _app->endRendering();
    }

    return 0;
}
