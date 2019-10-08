
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

    auto _box = engine::CMeshBuilder::createBox( 3.0f, 3.0f, 3.0f );
    auto _sphere = engine::CMeshBuilder::createSphere( 1.5f );
    auto _gizmo = engine::CMeshBuilder::createBox( 0.2f, 0.2f, 0.2f );
    _gizmo->position = { 0.0f, 0.0f, 2.0f };

    /* load the shader used for this example */
    std::string _baseNamePhong = std::string( ENGINE_EXAMPLES_PATH ) + "lights/shaders/phong";
    auto _shaderPhong = engine::CShaderManager::CreateShaderFromFiles( "phong_shader",
                                                                       _baseNamePhong + "_vs.glsl",
                                                                       _baseNamePhong + "_fs.glsl" );

    std::string _baseNameGouraud = std::string( ENGINE_EXAMPLES_PATH ) + "lights/shaders/gouraud";
    auto _shaderGouraud = engine::CShaderManager::CreateShaderFromFiles( "gouraud_shader",
                                                                         _baseNameGouraud + "_vs.glsl",
                                                                         _baseNameGouraud + "_fs.glsl" );

    ENGINE_ASSERT( _shaderPhong, "Could not load phong shader for our tests :(" );
    ENGINE_ASSERT( _shaderGouraud, "Could not load gouraud shader for our tests :(" );

    /* grab a simple shader to render the camera gizmo */
    auto _shaderGizmo = engine::CShaderManager::GetCachedShader( "basic3d_no_textures" );
    ENGINE_ASSERT( _shaderGizmo, "Could not grab the basic3d shader to render the light gizmo :(" );

    // select shader to use
    auto _shaderLighting = _shaderPhong;
    auto _mesh = _sphere;
    // _mesh->position = { 1.0f, 1.0f, 0.0f };
    // _mesh->scale = { 0.5f, 1.0f, 1.5f };

    bool _moveLight = false;
    float _mvParam = 0.0f;

    while( _app->active() )
    {
        if ( engine::CInputHandler::IsKeyDown( ENGINE_KEY_ESCAPE ) )
        {
            break;
        }
        else if ( engine::CInputHandler::CheckSingleKeyPress( ENGINE_KEY_S ) )
        {
            _shaderLighting = ( _shaderLighting->name() == "phong_shader" ) ? _shaderGouraud : _shaderPhong;
            ENGINE_INFO( "Using shader: {0}", _shaderLighting->name() );
        }
        else if ( engine::CInputHandler::CheckSingleKeyPress( ENGINE_KEY_P ) )
        {
            _moveLight = !_moveLight;
            ENGINE_INFO( "Light state: {0}", ( _moveLight ) ? "moving" : "fixed" );
        }

        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        _app->begin();
        _camera->update();

        if ( _moveLight )
        {
            _mvParam += 100.0f * engine::CTime::GetTimeStep();
            // _gizmo->position.x = 1.0f + std::sin( _mvParam ) * 2.0f;
            // _gizmo->position.y = std::sin( _mvParam / 2.0f ) * 1.0f;

            _gizmo->position.x = 10.0f * std::sin( _mvParam );
            _gizmo->position.y = 10.0f * std::cos( _mvParam );
            _gizmo->position.z = 0.0f;
        }

        /* do our thing here ************************/
        _shaderLighting->bind();
        _shaderLighting->setMat4( "u_modelMat", _mesh->matModel() );
        _shaderLighting->setMat4( "u_viewProjMat", _camera->matProj() * _camera->matView() );
        _shaderLighting->setMat4( "u_normalMat", ( ( _mesh->matModel() ).inverse() ).transpose() );
        _shaderLighting->setVec3( "u_objectColor", { 1.0f, 0.5f, 0.31f } );
        _shaderLighting->setVec3( "u_lightColor", { 1.0f, 1.0f, 1.0f } );
        _shaderLighting->setVec3( "u_lightPosition", _gizmo->position );
        _shaderLighting->setVec3( "u_viewerPosition", _camera->position() );

        _mesh->render();

        _shaderLighting->unbind();

        _shaderGizmo->bind();
        _shaderGizmo->setMat4( "u_tModel", _gizmo->matModel() );
        _shaderGizmo->setMat4( "u_tView", _camera->matView() );
        _shaderGizmo->setMat4( "u_tProj", _camera->matProj() );
        _shaderGizmo->setVec3( "u_color", { 1.0f, 1.0f, 1.0f } );

        _gizmo->render();

        _shaderGizmo->unbind();
        /********************************************/

        // engine::CDebugDrawer::DrawNormals( _mesh, { 0.0f, 0.0f, 1.0f } );
        engine::CDebugDrawer::Render( _camera );

        _app->end();

        // ENGINE_TRACE( "frame-time: {0}", engine::CTime::GetRawTimeStep() );
    }

    return 0;
}