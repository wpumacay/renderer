#pragma once

#include <common_py.h>
#include <core/CVertexBufferLayout.h>
#include <core/CVertexBuffer.h>
#include <core/CIndexBuffer.h>
#include <core/CVertexArray.h>

namespace py = pybind11;

namespace engine
{
    void bindings_bufferLayout( py::module& m );
    void bindings_vertexBuffer( py::module& m );
    void bindings_indexBuffer( py::module& m );
    void bindings_vertexArray( py::module& m );
}
