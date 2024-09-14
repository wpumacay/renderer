
#include <renderer/engine/graphics/enums.hpp>

namespace renderer {

auto ToString(eGraphicsAPI api) -> std::string {
    switch (api) {
        case eGraphicsAPI::OPENGL:
            return "opengl";
        case eGraphicsAPI::VULKAN:
            return "vulkan";
        case eGraphicsAPI::DIRECTX11:
            return "directx11";
        case eGraphicsAPI::DIRECTX12:
            return "directx12";
        default:
            return "none";
    }
}

auto ToString(eWindowBackend type) -> std::string {
    switch (type) {
        case eWindowBackend::TYPE_NONE:
            return "none";
        case eWindowBackend::TYPE_GLFW:
            return "glfw";
        case eWindowBackend::TYPE_EGL:
            return "egl";
        default:
            return "none";
    }
}

auto ToString(eShaderType type) -> std::string {
    switch (type) {
        case eShaderType::VERTEX:
            return "vertex";
        case eShaderType::FRAGMENT:
            return "fragment";
        case eShaderType::GEOMETRY:
            return "geometry";
        case eShaderType::COMPUTE:
            return "compute";
        default:
            return "vertex";
    }
}

}  // namespace renderer
