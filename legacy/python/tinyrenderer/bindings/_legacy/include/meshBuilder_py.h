#pragma once

#include <common_py.h>
#include <graphics/CMeshBuilder.h>

namespace py = pybind11;

namespace engine
{
    void bindings_meshBuilder( py::module& m );
}
