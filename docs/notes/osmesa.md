# Some notes on developing with support for OSMesa

So far we use two backends for rendering (both using OpenGL):

* `GLFW`: creates a window in an agnostic-way (X11, windows-API, etc.)
* `EGL`: creates a context for rendering without a window (**headless**)

The worst case scenario would be that one might not even have a GPU to render the
scene to. For these cases we will make use of [`OSMesa`][0] (**Mesa**'s off-screen
rendering interface), which in turn makes use of the CPU (software-rendering).

[Here][2] there is an example of how to use the API to create a GL-context. It
looks like we are the ones responsible for allocating the frame-buffer and handling
the render calls.


[0]: <https://docs.mesa3d.org/osmesa.html#> (osmesa-docs)
[1]: <https://gitlab.freedesktop.org/mesa/demos> (mesa-demos-repository)
[2]: <https://gitlab.freedesktop.org/mesa/demos/-/blob/main/src/osdemos/osdemo.c> (osmesa-sample-demo)
