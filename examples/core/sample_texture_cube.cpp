
#include <CEngine.h>

#include <core/CTexture.h>
#include <assets/CTextureManager.h>

engine::CMat4 computeSkyboxCorrectionMat( const engine::eAxis& axis )
{
    if ( axis == engine::eAxis::X ) return engine::CMat4( tinymath::rotationZ<float>( -ENGINE_PI / 2.0f ), { 0.0f, 0.0f, 0.0f } );
    else if ( axis == engine::eAxis::Z ) return engine::CMat4( tinymath::rotationX<float>( ENGINE_PI / 2.0f ), { 0.0f, 0.0f, 0.0f } );

    return engine::CMat4();
}

int main()
{
    auto _app = new engine::CApplication();

    auto _textureCubeDataCloudtop   = engine::CTextureManager::GetCachedTextureCubeData( "cloudtop" );
    auto _textureCubeDataStarfield  = engine::CTextureManager::GetCachedTextureCubeData( "starfield" );
    auto _textureCubeDataSiege      = engine::CTextureManager::GetCachedTextureCubeData( "siege" );

    if ( _textureCubeDataCloudtop )
        std::cout << engine::toString( *_textureCubeDataCloudtop ) << std::endl;
    
    if ( _textureCubeDataStarfield )
        std::cout << engine::toString( *_textureCubeDataStarfield ) << std::endl;

    if ( _textureCubeDataSiege )
        std::cout << engine::toString( *_textureCubeDataSiege ) << std::endl;

    auto _textureCubeCloudtop = engine::CTextureManager::GetCachedTextureCube( "cloudtop" );
    auto _textureCubeStarfield = engine::CTextureManager::GetCachedTextureCube( "starfield" );
    auto _textureCubeSiege = engine::CTextureManager::GetCachedTextureCube( "siege" );
    ENGINE_ASSERT( _textureCubeCloudtop, "Could not retrieve valid texture-cube for the sample - cloudtop" );
    ENGINE_ASSERT( _textureCubeStarfield, "Could not retrieve valid texture-cube for the sample - starfield" );
    ENGINE_ASSERT( _textureCubeSiege, "Could not retrieve valid texture-cube for the sample - siege" );

    auto _shader = engine::CShaderManager::GetCachedShader( "skybox" );

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
    _cameraProjData.aspect      = _app->window()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 100.0f;

    auto _camera = new engine::COrbitCamera( "orbit",
                                             engine::CVec3( 2.0f, 2.0f, 2.0f ),
                                             engine::CVec3( 0.0f, 0.0f, 0.0f ),
                                             engine::eAxis::Z,
                                             _cameraProjData,
                                             _app->window()->width(),
                                             _app->window()->height() );

    _scene->addCamera( std::unique_ptr< engine::CICamera >( _camera ) );

    auto _textureCubemap = _textureCubeCloudtop;
    // auto _textureCubemap = _textureCubeStarfield;
    // auto _textureCubemap = _textureCubeSiege;

    auto _correctionMat = computeSkyboxCorrectionMat( _camera->upAxis() );

    while( _app->active() )
    {
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        _app->update();

        _app->begin();
        _app->render();

        if ( engine::CInputManager::IsKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        glDepthFunc( GL_LEQUAL );

        _shader->bind();
        _shader->setMat4( "u_tProj", _camera->matProj() );
        _shader->setMat4( "u_tView", engine::CMat4( engine::CMat3( _camera->matView() ) ) * _correctionMat );

        _textureCubemap->bind();
        _varray->bind();

        glDrawArrays( GL_TRIANGLES, 0, 36 );

        _varray->unbind();
        _textureCubemap->unbind();

        _shader->unbind();

        glDepthFunc( GL_LESS );

        _app->end();
    }

    return 0;
}