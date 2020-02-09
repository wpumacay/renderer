#pragma once

#include <common_py.h>
#include <utils/CTime.h>

namespace py = pybind11;

namespace engine
{
    void bindings_utilTime( py::module& m );
}