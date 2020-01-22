#pragma once

#include <common_py.h>
#include <shaders/CShader.h>
#include <shaders/CShaderManager.h>

namespace py = pybind11;

namespace engine
{
    void bindings_shader( py::module& m );
    void bindings_shaderManager( py::module& m );
}