#pragma once

#include <common_py.h>
#include <gl/COpenGLWindow.h>

namespace py = pybind11;

namespace engine
{
    void bindings_window( py::module& m );
}