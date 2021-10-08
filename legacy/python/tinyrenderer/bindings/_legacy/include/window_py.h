#pragma once

#include <common_py.h>

#include <gl/CIWindow.h>
#ifndef ENGINE_HEADLESS_EGL
    #include <gl/CWindowGLFW.h>
#else
    #include <gl/CWindowEGL.h>
#endif

namespace py = pybind11;

namespace engine
{
    void bindings_windowBase( py::module& m );
#ifndef ENGINE_HEADLESS_EGL
    void bindings_windowGLFW( py::module& m );
#else
    void bindings_windowEGL( py::module& m );
#endif /* ENGINE_HEADLESS_EGL */
}
