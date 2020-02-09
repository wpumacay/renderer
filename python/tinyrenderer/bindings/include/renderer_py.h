#pragma once

#include <common_py.h>
#include <renderers/CRenderersCommon.h>
#include <renderers/CMainRenderer.h>
#include <renderers/CMeshRenderer.h>
#include <renderers/CSkyboxRenderer.h>

namespace py = pybind11;

namespace engine
{
    void bindings_renderers( py::module& m );
}