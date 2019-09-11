
#include <iostream>

#include <core/COpenGLApp.h>
#include <utils/CLogger.h>

int main()
{
    auto _window = new engine::COpenGLApp();

    while( _window->isActive() )
    {
        _window->begin();

        if ( engine::InputSystem::isKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        _window->end();
    }

    return 0;
}
