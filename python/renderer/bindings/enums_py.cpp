#include <pybind11/pybind11.h>

#include <renderer/engine/graphics/enums.hpp>

namespace py = pybind11;

namespace renderer {

// NOLINTNEXTLINE
auto bindings_enums(py::module m) {
    {
        using Enum = ::renderer::eGraphicsAPI;
        py::enum_<Enum>(m, "GraphicsAPI")
            .value("OPENGL", Enum::OPENGL)
            .value("VULKAN", Enum::VULKAN)
            .value("DIRECTX11", Enum::DIRECTX11)
            .value("DIRECTX12", Enum::DIRECTX12);
    }

    {
        using Enum = ::renderer::eWindowBackend;
        py::enum_<Enum>(m, "WindowBackend")
            .value("TYPE_NONE", Enum::TYPE_NONE)
            .value("TYPE_GLFW", Enum::TYPE_GLFW)
            .value("TYPE_EGL", Enum::TYPE_EGL);
    }

    {
        using Enum = ::renderer::eShaderType;
        py::enum_<Enum>(m, "ShaderType")
            .value("VERTEX", Enum::VERTEX)
            .value("FRAGMENT", Enum::FRAGMENT)
            .value("GEOMETRY", Enum::GEOMETRY)
            .value("COMPUTE", Enum::COMPUTE);
    }

    {
        using Enum = ::renderer::eElementType;
        py::enum_<Enum>(m, "ElementType")
            .value("FLOAT_1", Enum::FLOAT_1)
            .value("FLOAT_2", Enum::FLOAT_2)
            .value("FLOAT_3", Enum::FLOAT_3)
            .value("FLOAT_4", Enum::FLOAT_4)
            .value("INT_1", Enum::INT_1)
            .value("INT_2", Enum::INT_2)
            .value("INT_3", Enum::INT_3)
            .value("INT_4", Enum::INT_4);
    }
}

}  // namespace renderer
