
#include <iostream>

#include <core/COpenGLApp.h>
#include <utils/CLogger.h>

int main()
{
    auto _app = new engine::COpenGLApp();
    _app->init();

    while( _app->isActive() )
    {
        _app->begin();

        if ( engine::InputSystem::isKeyDown( ENGINE_KEY_ESCAPE ) )
            break;

        _app->end();
    }

    return 0;
}
