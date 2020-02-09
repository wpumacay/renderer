#pragma once

#include <common_py.h>
#include <app/CApplication.h>

namespace py = pybind11;

namespace engine
{
    void bindings_application( py::module& m );
}