
#include <iostream>

#include <LWindow.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
