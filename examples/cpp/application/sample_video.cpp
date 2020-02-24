
#include <CEngine.h>

int main()
{
    auto _app = std::make_unique<engine::CApplication>();
    auto _decoder = std::make_unique<engine::CVideoDecoder>( engine::eDecodingMode::SYNC,
                                                             "/home/gregor/Documents/repos/cat1_workspace/cat1/frames/sample_movie.mp4" );

    while ( _app->active() )
    {
        if ( engine::CInputManager::IsKeyDown( engine::Keys::KEY_ESCAPE ) )
            break;

        _app->update();
        _app->begin();
        _app->render();

        _app->end();
    }

    return 0;
}