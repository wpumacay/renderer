
#include <iostream>

#include <LWindow.h>
#include <utils/CLogger.h>

int main()
{
    engine::CLogger::Init();

    auto _window = new engine::LWindow( { 1024, 768, "Application window" } );

    while( _window->isActive() )
    {
        _window->begin();

        // render some stuff here

        _window->end();
    }

    return 0;
}
