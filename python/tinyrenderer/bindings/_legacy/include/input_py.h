#pragma once

#include <common_py.h>
#include <input/CInputKey.h>
#include <input/CInputMouse.h>
#include <input/CInputManager.h>

namespace py = pybind11;

namespace engine
{
    void bindings_inputKey( py::module& m );
    void bindings_inputMouse( py::module& m );
    void bindings_inputManager( py::module& m );
}