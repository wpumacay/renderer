
#include <iostream>

#include <LWindow.h>
#include <LFixedCamera3d.h>
#include <LFpsCamera.h>
#include <LLightPoint.h>

#include <LScene.h>


int main()
{

    auto _window = engine::LWindow::GetInstance();


    while( _window->isActive() )
    {
        _window->clear();
        _window->pollEvents();

        // render some stuff here

        _window->swapBuffers();
    }

    return 0;
}
