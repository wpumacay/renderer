#pragma once

#include <common_py.h>
#include <CMath.h>

namespace py = pybind11;

namespace engine
{
    void bindings_mathHelpers( py::module& m );
}