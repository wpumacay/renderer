
#include <CEngine.h>

#include <core/CFrameBuffer.h>

int main()
{
    auto _app = new engine::COpenGLApp();
    _app->init();

    engine::CAttachmentConfig _fbColorConfig;
    _fbColorConfig.name                 = "color_attachment";
    _fbColorConfig.attachment           = engine::eFboAttachment::COLOR;
    _fbColorConfig.width                = engine::COpenGLApp::GetWindow()->width();
    _fbColorConfig.height               = engine::COpenGLApp::GetWindow()->height();
    _fbColorConfig.texInternalFormat    = engine::eTextureFormat::RGB;
    _fbColorConfig.texFormat            = engine::eTextureFormat::RGB;
    _fbColorConfig.texPixelDataType     = engine::ePixelDataType::UINT_8;
    _fbColorConfig.texWrapU             = engine::eTextureWrap::REPEAT;
    _fbColorConfig.texWrapV             = engine::eTextureWrap::REPEAT;

    engine::CAttachmentConfig _fbDepthConfig;
    _fbDepthConfig.name                 = "depth_attachment";
    _fbDepthConfig.attachment           = engine::eFboAttachment::DEPTH;
    _fbDepthConfig.width                = engine::COpenGLApp::GetWindow()->width();
    _fbDepthConfig.height               = engine::COpenGLApp::GetWindow()->height();
    _fbDepthConfig.texInternalFormat    = engine::eTextureFormat::DEPTH;
    _fbDepthConfig.texFormat            = engine::eTextureFormat::DEPTH;
    _fbDepthConfig.texPixelDataType     = engine::ePixelDataType::UINT_32;
    _fbDepthConfig.texWrapU             = engine::eTextureWrap::REPEAT;
    _fbDepthConfig.texWrapV             = engine::eTextureWrap::REPEAT;

    auto _framebuffer = new engine::CFrameBuffer();
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

    auto _quad_vbuffer = new engine::CVertexBuffer( _layout,
                                                    engine::eBufferUsage::STATIC,
                                                    sizeof( _quad_buffData ),
                                                    _quad_buffData );;

    auto _quad_ibuffer = new engine::CIndexBuffer( engine::eBufferUsage::STATIC,
                                                   6, _quad_indices );

    auto _quad_varray = new engine::CVertexArray();
    _quad_varray->addVertexBuffer( _quad_vbuffer );
    _quad_varray->setIndexBuffer( _quad_ibuffer );

    /* setup a simple scene to render to our target *********************************/

    auto _scene = _app->scene();

    auto _cameraProjData = engine::CCameraProjData();
    _cameraProjData.projection  = engine::eCameraProjection::PERSPECTIVE;
    _cameraProjData.fov         = 45.0f;
    _cameraProjData.aspect      = engine::COpenGLApp::GetWindow()->aspect();
    _cameraProjData.zNear       = 0.1f;
    _cameraProjData.zFar        = 100.0f;

    auto _camera = new engine::COrbitCamera( "orbit",
                                             { 4.0f, 4.0f, 4.0f },
                                             { 0.0f, 0.0f, 0.0f },
                                             engine::eAxis::Z,
                                             _cameraProjData,
                                             engine::COpenGLApp::GetWindow()->width(),
                                             engine::COpenGLApp::GetWindow()->height() );
    _scene->addCamera( _camera );

    auto _light = new engine::LLightDirectional( { 0.8f, 0.8f, 0.8f }, { 0.8f, 0.8f, 0.8f },
                                                 { 0.3f, 0.3f, 0.3f }, 0, { 0.0f, 0.0f, -1.0f } );
    _light->setVirtualPosition( { 5.0f, 0.0f, 5.0f } );
    _scene->addLight( _light );

    auto _plane = engine::CMeshBuilder::createPlane( 10.0f, 10.0f );
    _plane->getMaterial()->setColor( { 0.2f, 0.3f, 0.4f } );
    _scene->addRenderable( _plane );

    auto _box = engine::CMeshBuilder::createBox( 0.25f, 0.5f, 1.0f );
    _box->getMaterial()->setColor( { 0.7f, 0.5f, 0.3f } );
    _box->pos = { 1.0f, 1.0f, 1.0f };
    _scene->addRenderable( _box );

    std::cout << engine::toString( _framebuffer->getConfigAttachment( "color_attachment" ) ) << std::endl;
    std::cout << engine::toString( _framebuffer->getConfigAttachment( "depth_attachment" ) ) << std::endl;

    std::cout << engine::toString( *_framebuffer->getTextureAttachment( "color_attachment" )->data() ) << std::endl;
    std::cout << engine::toString( *_framebuffer->getTextureAttachment( "depth_attachment" )->data() ) << std::endl;

    _app->renderer()->disableShadows();

    while( _app->isActive() )
    {
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } );
        engine::CDebugDrawer::DrawLine( { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, { 0.0f, 0.0f, 1.0f } );

        if ( engine::CInputHandler::IsKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        /* Render to a custom target given by our framebuffer */

        // set the render target to our framebuffer
        _framebuffer->bind();

        glEnable( GL_DEPTH_TEST );

        _app->begin();

        // let the renderer to the thing :D
        _app->update();

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

        _app->end();
    }

    return 0;
}