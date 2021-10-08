#pragma once

#include <common_py.h>
#include <camera/CICamera.h>
#include <camera/CFixedCamera.h>
#include <camera/CFixedCamera.h>
#include <camera/CFpsCamera.h>
#include <camera/COrbitCamera.h>

namespace py = pybind11;

namespace engine
{
    void bindings_cameraBase( py::module& m );
    void bindings_cameraTypes( py::module& m );
}
