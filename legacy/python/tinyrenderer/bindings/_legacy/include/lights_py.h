#pragma once

#include <common_py.h>
#include <lights/CILight.h>
#include <lights/CDirectionalLight.h>
#include <lights/CPointLight.h>
#include <lights/CSpotLight.h>

namespace py = pybind11;

namespace engine
{
    void bindings_lightBase( py::module& m );
    void bindings_lightDirectional( py::module& m );
    void bindings_lightPoint( py::module& m );
    void bindings_lightSpot( py::module& m );
}
