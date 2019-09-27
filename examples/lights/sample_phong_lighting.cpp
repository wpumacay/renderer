
#include <CEngine.h>

int main()
{
    auto _app = new engine::COpenGLApp();
    _app->init();

    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = engine::COpenGLApp::GetWindow()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 100.0f;

    auto _camera = new engine::COrbitCamera( "orbit",
                                             { 0.0f, 0.0f, 3.0f },
                                             { 0.0f, 0.0f, 0.0f },
                                             engine::eAxis::Z,
                                             _cameraProjData,
                                             engine::COpenGLApp::GetWindow()->width(),
                                             engine::COpenGLApp::GetWindow()->height() );

//     const float _cameraSensitivity  = 0.25f;
//     const float _cameraSpeed        = 25.0f;
//     const float _cameraMaxDelta     = 10.0f;
//     auto _camera = new engine::CFpsCamera( "fps",
//                                            { 0.0f, 0.0f, 3.0f },
//                                            { 0.0f, 0.0f, 0.0f },
//                                            engine::eAxis::Z,
//                                            _cameraProjData,
//                                            _cameraSensitivity,
//                                            _cameraSpeed,
//                                            _cameraMaxDelta );

    auto _box = engine::CMeshBuilder::createBox( 1.0f, 1.0f, 1.0f );
    auto _gizmo = engine::CMeshBuilder::createBox( 0.2f, 0.2f, 0.2f );
    _gizmo->pos = { 0.0f, 0.0f, 2.0f };

    /* load the shader used for this example */
    std::string _baseName = std::string( ENGINE_EXAMPLES_PATH ) + "lights/shaders/phong";
    auto _shaderPhong = engine::CShaderManager::CreateShaderFromFiles( "phong_shader",
                                                                       _baseName + "_vs.glsl",
                                                                       _baseName + "_fs.glsl" );

    ENGINE_ASSERT( _shaderPhong, "Could not load phong shader for our tests :(" );

    /* grab a simple shader to render the camera gizmo */
    auto _shaderGizmo = engine::CShaderManager::GetCachedShader( "basic3d_no_textures" );
    ENGINE_ASSERT( _shaderGizmo, "Could not grab the basic3d shader to render the light gizmo :(" );

    while( _app->isActive() )
    {
        if ( engine::CInputHandler::IsKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        _app->begin();
        _camera->update();

        _gizmo->pos.x = 1.0f + std::sin( glfwGetTime() ) * 2.0f;
        _gizmo->pos.y = std::sin( glfwGetTime() / 2.0f ) * 1.0f;

        /* do our thing here ************************/
        _shaderPhong->bind();
        _shaderPhong->setMat4( "u_modelMat", _box->getModelMatrix() );
        _shaderPhong->setMat4( "u_viewProjMat", _camera->matProj() * _camera->matView() );
        _shaderPhong->setMat4( "u_normalMat", ( ( _box->getModelMatrix() ).inverse() ).transpose() );
        _shaderPhong->setVec3( "u_objectColor", { 1.0f, 0.5f, 0.31f } );
        _shaderPhong->setVec3( "u_lightColor", { 1.0f, 1.0f, 1.0f } );
        _shaderPhong->setVec3( "u_lightPosition", _gizmo->pos );
        _shaderPhong->setVec3( "u_viewerPosition", _camera->matView().getPosition() );

        _box->render();

        _shaderPhong->unbind();

        _shaderGizmo->bind();
        _shaderGizmo->setMat4( "u_tModel", _gizmo->getModelMatrix() );
        _shaderGizmo->setMat4( "u_tView", _camera->matView() );
        _shaderGizmo->setMat4( "u_tProj", _camera->matProj() );
        _shaderGizmo->setVec3( "u_color", { 1.0f, 1.0f, 1.0f } );

        _gizmo->render();

        _shaderGizmo->unbind();
        /********************************************/

        _app->end();
    }

    return 0;
}