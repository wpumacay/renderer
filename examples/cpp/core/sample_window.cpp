
#include <window/CWindowGLFW.h>

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

void resizeCallback( int newWidth, int newHeight )
{
    ENGINE_TRACE( "window-width: {0}, window-height: {1}", newWidth, newHeight );
}

int main()
{
    tinyutils::Logger::Init();

    auto window_props = engine::CWindowProps();
    window_props.width = 800;
    window_props.height = 600;
    window_props.title = "sample-window-c++";
    window_props.resizable = true;
    window_props.clearColor = { 0.2f, 0.4f, 0.8f, 1.0f };

    auto window = engine::CWindowGLFW::Create( window_props );
    window->RegisterKeyCallback( keyCallback );
    window->RegisterMouseCallback( mouseCallback );
    window->RegisterMouseMoveCallback( mouseMoveCallback );
    window->RegisterScrollCallback( scrollCallback );
    window->RegisterResizeCallback( resizeCallback );

    while( window->Active() )
    {
        window->Begin();
        if ( g_terminate )
            window->RequestClose();
        window->End();
    }
    return 0;
}
