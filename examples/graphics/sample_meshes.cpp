
#include <CEngine.h>

void renderScene( engine::CShader* shaderPtr,
                  engine::CICamera* cameraPtr,
                  engine::CILight* lightPtr,
                  const std::vector< engine::CIRenderable* >& renderables );

int main()
{
    auto _app = new engine::COpenGLApp();
    _app->init();

    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = engine::COpenGLApp::GetWindow()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 50.0f;

    auto _camera = new engine::COrbitCamera( "orbit",
                                             { 0.0f, 0.0f, 3.0f },
                                             { 0.0f, 0.0f, 0.0f },
                                             engine::eAxis::Y,
                                             _cameraProjData,
                                             engine::COpenGLApp::GetWindow()->width(),
                                             engine::COpenGLApp::GetWindow()->height() );



    while( _app->active() )
    {
        if ( engine::CInputHandler::CheckSingleKeyPress( ENGINE_KEY_ESCAPE ) )
            break;

        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        _app->begin();

        /* do our thing here ************************/

        // render the scene
        // renderScene();

        /********************************************/

        engine::CDebugDrawer::Render( _camera );

        _app->end();
    }

    return 0;
}