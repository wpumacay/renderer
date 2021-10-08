#pragma once

#include <common_py.h>
#include <core/CTexture.h>
#include <core/CTextureCube.h>
#include <assets/CTextureManager.h>

namespace py = pybind11;

namespace engine
{
    void bindings_texture( py::module& m );
    void bindings_textureCube( py::module& m );
    void bindings_textureManager( py::module& m );
}
