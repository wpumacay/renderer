
#include <CEngine.h>

#include <core/CTexture.h>
#include <assets/CTextureManager.h>

class Application : public engine::COpenGLApp
{

public :

    Application() : engine::COpenGLApp() {}
    ~Application() {}

protected :

    void _initUser() override
    {
        ENGINE_TRACE( "Initializing custom ui" );
        m_uiPtr = new engine::CImguiUiDemo( m_windowPtr->context() );
        m_uiPtr->init();
    }

};


int main()
{
    auto _app = new Application();
    _app->init();

    auto _textureCubeDataCloudtop   = engine::CTextureManager::GetCachedTextureCubeData( "cloudtop" );
    auto _textureCubeDataStarfield  = engine::CTextureManager::GetCachedTextureCubeData( "starfield" );

    if ( _textureCubeDataCloudtop )
        std::cout << engine::toString( *_textureCubeDataCloudtop ) << std::endl;
    
    if ( _textureCubeDataStarfield )
        std::cout << engine::toString( *_textureCubeDataStarfield ) << std::endl;

    auto _textureCubeCloudtop = engine::CTextureManager::GetCachedTextureCube( "cloudtop" );
    auto _textureCubeStarfield = engine::CTextureManager::GetCachedTextureCube( "starfield" );
    ENGINE_ASSERT( _textureCubeCloudtop, "Could not retrieve valid texture-cube for the sample - cloudtop" );
    ENGINE_ASSERT( _textureCubeStarfield, "Could not retrieve valid texture-cube for the sample - starfield" );

    auto _shader = engine::LShaderManager::getShader( "skybox" );

    engine::float32 _cubemapVertices[] = {
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
    };

    auto _vbuffer = new engine::CVertexBuffer( { { "position", engine::eElementType::Float3, false } },
                                               engine::eBufferUsage::STATIC,
                                               sizeof( _cubemapVertices ),
                                               _cubemapVertices );

    auto _varray = new engine::CVertexArray();
    _varray->addVertexBuffer( _vbuffer );

    auto _scene = _app->scene();

    // make a sample camera
    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = engine::COpenGLApp::GetWindow()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 100.0f;

    auto _camera = new engine::COrbitCamera( "orbit",
                                             engine::CVec3( 2.0f, 2.0f, 2.0f ),
                                             engine::CVec3( 0.0f, 0.0f, 0.0f ),
                                             engine::eAxis::Z,
                                             _cameraProjData,
                                             engine::COpenGLApp::GetWindow()->width(),
                                             engine::COpenGLApp::GetWindow()->height() );

    _scene->addCamera( _camera );

    // auto _textureCubemap = _textureCubeCloudtop;
    auto _textureCubemap = _textureCubeStarfield;

    while( _app->isActive() )
    {
        engine::DebugSystem::drawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::DebugSystem::drawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::DebugSystem::drawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        _app->begin();

        if ( engine::InputSystem::isKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        glDepthFunc( GL_LEQUAL );

        _shader->bind();
        _shader->setMat4( "u_tProj", _camera->matProj() );
        _shader->setMat4( "u_tView", _camera->matView().getRotation() );

        _textureCubemap->bind();
        _varray->bind();

        glDrawArrays( GL_TRIANGLES, 0, 36 );

        _varray->unbind();
        _textureCubemap->unbind();

        _shader->unbind();

        glDepthFunc( GL_LESS );

        _app->update();
        _app->end();
    }

    return 0;
}