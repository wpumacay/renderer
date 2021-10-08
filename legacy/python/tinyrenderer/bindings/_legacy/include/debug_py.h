#pragma once

#include <common_py.h>
#include <utils/CDebugDrawer.h>

namespace py = pybind11;

namespace engine
{
    void bindings_debugDrawer( py::module& m );
}
