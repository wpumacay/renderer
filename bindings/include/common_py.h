#pragma once

#include <pybind11/pybind11.h>
#include <CCommon.h>
#include <CMath.h>

namespace py = pybind11;

namespace engine
{
    void bindings_common( py::module& m );
}