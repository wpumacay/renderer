#pragma once

#include <common_py.h>
#include <core/CFrameBuffer.h>

namespace py = pybind11;

namespace engine
{
    void bindings_frameBuffer( py::module& m );
}