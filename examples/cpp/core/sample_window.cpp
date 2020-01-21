
#include <CEngine.h>

bool g_terminate = false;
int g_numClicks = 0;

void keyCallback( int key, int action )
{
    ENGINE_TRACE( "key: {0}, action: {1}", key, action );
    g_terminate = ( ( key == engine::Keys::KEY_ESCAPE ) && ( action == engine::KeyAction::KEY_PRESSED ) );
}

void mouseCallback( int button, int action, double x, double y )
{
    ENGINE_TRACE( "button: {0}, action: {1}, x: {2}, y: {3}", button, action, x, y );
    if ( ( button == engine::Mouse::BUTTON_LEFT ) && ( action == engine::MouseAction::BUTTON_PRESSED ) )
        g_numClicks++;
    ENGINE_TRACE( "num-clicks: {0}", g_numClicks );
}

void mouseMoveCallback( double x, double y )
{
    ENGINE_TRACE( "x: {0}, y: {1}", x, y );
}

void scrollCallback( double xOff, double yOff )
{
    ENGINE_TRACE( "xOff: {0}, y: {1}", xOff, yOff );
}

int main()
{
    engine::CLogger::Init();

    auto _window = std::unique_ptr< engine::COpenGLWindow >( new engine::COpenGLWindow( engine::CWindowProps() ) );
    _window->registerKeyCallback( keyCallback );
    _window->registerMouseCallback( mouseCallback );
    _window->registerMouseMoveCallback( mouseMoveCallback );
    _window->registerScrollCallback( scrollCallback );

    while( _window->active() )
    {
        _window->begin();

        if ( g_terminate )
            _window->requestClose();

        _window->end();
    }

    return 0;
}
