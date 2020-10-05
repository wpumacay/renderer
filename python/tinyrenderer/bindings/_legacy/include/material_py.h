#pragma once

#include <common_py.h>
#include <materials/CMaterial.h>

namespace py = pybind11;

namespace engine
{
    void bindings_material( py::module& m );
}