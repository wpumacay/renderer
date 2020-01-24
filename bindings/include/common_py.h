#pragma once

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <CCommon.h>
#include <CMath.h>
// binding declarations from tinymath
#include <bindings_vector_t.h>
#include <bindings_matrix_t.h>

namespace py = pybind11;

namespace engine
{
    void bindings_common( py::module& m );
}