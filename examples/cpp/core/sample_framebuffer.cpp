
#include <CEngine.h>

int main()
{
    auto _app = std::make_unique<engine::CApplication>();

    engine::CAttachmentConfig _fbColorConfig;
    _fbColorConfig.name                 = "color_attachment";
    _fbColorConfig.attachment           = engine::eFboAttachment::COLOR;
    _fbColorConfig.width                = _app->window()->width();
    _fbColorConfig.height               = _app->window()->height();
    _fbColorConfig.texInternalFormat    = engine::eTextureFormat::RGB;
    _fbColorConfig.texFormat            = engine::eTextureFormat::RGB;
    _fbColorConfig.texPixelDataType     = engine::ePixelDataType::UINT_8;
    _fbColorConfig.texWrapU             = engine::eTextureWrap::REPEAT;
    _fbColorConfig.texWrapV             = engine::eTextureWrap::REPEAT;

    engine::CAttachmentConfig _fbDepthConfig;
    _fbDepthConfig.name                 = "depth_attachment";
    _fbDepthConfig.attachment           = engine::eFboAttachment::DEPTH;
    _fbDepthConfig.width                = _app->window()->width();
    _fbDepthConfig.height               = _app->window()->height();
    _fbDepthConfig.texInternalFormat    = engine::eTextureFormat::DEPTH;
    _fbDepthConfig.texFormat            = engine::eTextureFormat::DEPTH;
    _fbDepthConfig.texPixelDataType     = engine::ePixelDataType::UINT_32;
    _fbDepthConfig.texWrapU             = engine::eTextureWrap::REPEAT;
    _fbDepthConfig.texWrapV             = engine::eTextureWrap::REPEAT;

    auto _framebuffer = std::make_unique<engine::CFrameBuffer>();
    _framebuffer->addAttachment( _fbColorConfig );
    _framebuffer->addAttachment( _fbDepthConfig );

    engine::float32 _quad_buffData[] = {
     /*|  positions |      colors     |     uvs    |*/
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };

    engine::uint32 _quad_indices[] = {
        0, 1, 2, 
        0, 2, 3
    };

    engine::CVertexBufferLayout _layout = { { "pos", engine::eElementType::Float2, false },
                                            { "color", engine::eElementType::Float3, false },
                                            { "uv", engine::eElementType::Float2, false } };

    auto _quad_vbuffer = std::make_unique<engine::CVertexBuffer>( _layout, engine::eBufferUsage::STATIC,
                                                                  sizeof( _quad_buffData ), _quad_buffData );;

    auto _quad_ibuffer = std::make_unique<engine::CIndexBuffer>( engine::eBufferUsage::STATIC, 6, _quad_indices );

    auto _quad_varray = std::make_unique<engine::CVertexArray>();
    _quad_varray->addVertexBuffer( std::move( _quad_vbuffer ) );
    _quad_varray->setIndexBuffer( std::move( _quad_ibuffer ) );

    /* setup a simple scene to render to our target *********************************/

    auto _scene = _app->scene();

    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = _app->window()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 100.0f;

    auto _camera = std::make_unique<engine::COrbitCamera>( "orbit",
                                                           engine::CVec3( 4.0f, 4.0f, 4.0f ),
                                                           engine::CVec3( 0.0f, 0.0f, 0.0f ),
                                                           engine::eAxis::Z,
                                                           _cameraProjData,
                                                           _app->window()->width(),
                                                           _app->window()->height() );
    _scene->addCamera( std::move( _camera ) );

    auto _light = std::make_unique<engine::CDirectionalLight>( "directional_1",
                                                               engine::CVec3( 0.8f, 0.8f, 0.8f ), 
                                                               engine::CVec3( 0.8f, 0.8f, 0.8f ),
                                                               engine::CVec3( 0.3f, 0.3f, 0.3f ), 
                                                               engine::CVec3( 0.0f, 0.0f, -1.0f ) );
    _scene->addLight( std::move( _light ) );

    auto _plane = engine::CMeshBuilder::createPlane( 10.0f, 10.0f );
    _plane->material()->ambient  = { 0.2f, 0.3f, 0.4f };
    _plane->material()->diffuse  = { 0.2f, 0.3f, 0.4f };
    _plane->material()->specular = { 0.2f, 0.3f, 0.4f };
    _scene->addRenderable( std::unique_ptr< engine::CIRenderable >( _plane ) );

    auto _box = engine::CMeshBuilder::createBox( 0.25f, 0.5f, 1.0f );
    _box->material()->ambient  = { 0.7f, 0.5f, 0.3f };
    _box->material()->diffuse  = { 0.7f, 0.5f, 0.3f };
    _box->material()->specular = { 0.7f, 0.5f, 0.3f };
    _box->position = { 1.0f, 1.0f, 1.0f };
    _scene->addRenderable( std::unique_ptr< engine::CIRenderable >( _box ) );

    std::cout << engine::toString( _framebuffer->getConfigAttachment( "color_attachment" ) ) << std::endl;
    std::cout << engine::toString( _framebuffer->getConfigAttachment( "depth_attachment" ) ) << std::endl;

    std::cout << engine::toString( *_framebuffer->getTextureAttachment( "color_attachment" )->data() ) << std::endl;
    std::cout << engine::toString( *_framebuffer->getTextureAttachment( "depth_attachment" )->data() ) << std::endl;

    // _app->renderer()->disableShadows();

    while( _app->active() )
    {
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        if ( engine::CInputManager::IsKeyDown( engine::Keys::KEY_ESCAPE ) )
            break;

        _app->update();

        /* Render to a custom target given by our framebuffer */

        // set the render target to our framebuffer
        _framebuffer->bind();

        _app->begin();

        // let the renderer to the thing :D
        _app->render();

        // release our render targets
        _framebuffer->unbind();

        /* Render what's inside our framebuffer into a quad on the screen */

        glDisable( GL_DEPTH_TEST );
        glClearColor( 0.5f, 0.5f, 0.5f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );
        auto _shaderTex2d = engine::CShaderManager::GetCachedShader( "basic2d_textures" );
        ENGINE_ASSERT( _shaderTex2d, "No shader to render our render target texture into a quad :(" );

        _shaderTex2d->bind();
        _framebuffer->getTextureAttachment( "color_attachment" )->bind();
        _quad_varray->bind();

        glDrawElements( GL_TRIANGLES, _quad_varray->indexBuffer()->count(), GL_UNSIGNED_INT, 0 );

        _quad_varray->unbind();
        _framebuffer->getTextureAttachment( "color_attachment" )->unbind();
        _shaderTex2d->unbind();
        glEnable( GL_DEPTH_TEST );

        _app->end();
    }

    return 0;
}