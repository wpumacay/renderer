
#include <CEngine.h>

int main()
{
#ifdef ENGINE_USE_FFMPEG
    auto _app = std::make_unique<engine::CApplication>();
    auto _decoder = std::make_unique<engine::CVideoDecoder>( engine::eDecodingMode::SYNC,
                                                             "/home/gregor/Documents/repos/cat1_workspace/cat1/tmp/frames/sample_movie.mp4" );

    // Setup resources to show a simple texture with the decoder frame *****************************
    float _bufferData[] = {
    /*|      pos     |     color      |  texture  */
        -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };

    engine::uint32 _indices[] = { 
        0, 1, 2, // first triangle
        0, 2, 3  // second triangle
    };

    engine::CVertexBufferLayout _layout = { { "pos", engine::eElementType::Float2, false }, 
                                            { "col", engine::eElementType::Float3, false },
                                            { "uv",  engine::eElementType::Float2, false } };

    auto _vbuffer = std::make_unique<engine::CVertexBuffer>( _layout, engine::eBufferUsage::STATIC,
                                                             sizeof( _bufferData ), _bufferData );
    auto _ibuffer = std::make_unique<engine::CIndexBuffer>( engine::eBufferUsage::STATIC, 6, _indices );

    auto _varray = std::make_unique<engine::CVertexArray>();
    _varray->addVertexBuffer( std::move( _vbuffer ) );
    _varray->setIndexBuffer( std::move( _ibuffer ) );

    auto textureData = std::make_unique<engine::CTextureData>();
    textureData->name           = "video_tex_data";
    textureData->data           = nullptr; // texture's actual data is set with the video-decoder output
    textureData->width          = 0; // this is set from the video-decoder properties
    textureData->height         = 0; // this is set from the video-decoder properties
    textureData->channels       = 0; // this is set from the video-decoder properties
    textureData->internalFormat = engine::eTextureFormat::RGB;
    textureData->format         = engine::eTextureFormat::RGB;

    engine::CTextureOptions textureOptions;
    textureOptions.filterMin       = engine::eTextureFilter::LINEAR;
    textureOptions.filterMag       = engine::eTextureFilter::LINEAR;
    textureOptions.wrapU           = engine::eTextureWrap::REPEAT;
    textureOptions.wrapV           = engine::eTextureWrap::REPEAT;
    textureOptions.borderColorU    = engine::CVec4( 0.0f, 0.0f, 0.0f, 1.0f );
    textureOptions.borderColorV    = engine::CVec4( 0.0f, 0.0f, 0.0f, 1.0f );
    textureOptions.dtype           = engine::ePixelDataType::UINT_8;

    auto video_texture = std::make_unique<engine::CTexture>( std::move( textureData ), textureOptions );

    auto _shader = engine::CShaderManager::GetCachedShader( "basic2d_textures" );
    //**********************************************************************************************

    while ( _app->active() )
    {
        tinyutils::Clock::Tick();
        if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_ESCAPE ) )
            break;
        //// else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_D ) )
        //// {
        ////     auto next_frame = _decoder->GetNextFrame();
        ////     video_texture->resize( _decoder->GetCurrentFrameWidth(),
        ////                            _decoder->GetCurrentFrameHeight(),
        ////                            (engine::uint8*) next_frame.get() );
        //// }

        _app->update();
        _app->begin();

        _shader->bind();
        _varray->bind();

        auto next_frame = _decoder->GetNextFrame();
        video_texture->resize( _decoder->GetCurrentFrameWidth(),
                               _decoder->GetCurrentFrameHeight(),
                               (engine::uint8*) next_frame.get() );
        video_texture->bind();

        glDrawElements( GL_TRIANGLES, _varray->indexBuffer()->count(), GL_UNSIGNED_INT, 0 );

        video_texture->unbind();

        _varray->unbind();
        _shader->unbind();

        _app->render();

        _app->end();
        tinyutils::Clock::Tock();
    }
#endif
    return 0;
}