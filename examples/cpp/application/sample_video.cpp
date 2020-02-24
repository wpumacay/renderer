
#include <CEngine.h>

int main()
{
#ifdef ENGINE_USE_FFMPEG
    auto _app = std::make_unique<engine::CApplication>();
    auto _decoder = std::make_unique<engine::CVideoDecoder>( engine::eDecodingMode::SYNC,
                                                             "/home/gregor/Documents/repos/cat1_workspace/cat1/tmp/frames/sample_movie.mp4" );

    while ( _app->active() )
    {
        if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_ESCAPE ) )
            break;
        else if ( engine::CInputManager::CheckSingleKeyPress( engine::Keys::KEY_D ) )
            _decoder->GetNextFrame();

        _app->update();
        _app->begin();
        _app->render();

        _app->end();
    }
#endif
    return 0;
}