#pragma once

#include <common_py.h>
#include <graphics/CScene.h>

namespace py = pybind11;

namespace engine
{
    void bindings_scene( py::module& m );
}