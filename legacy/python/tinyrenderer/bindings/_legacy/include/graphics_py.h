#pragma once

#include <common_py.h>
#include <graphics/CFog.h>
#include <graphics/CSkybox.h>
#include <graphics/CShadowMap.h>

namespace py = pybind11;

namespace engine
{
    void bindings_fog( py::module& m );
    void bindings_skybox( py::module& m );
    void bindings_shadowMap( py::module& m );
}
