#include <renderer/engine/graphics/window_config_t.hpp>

namespace renderer {

auto ToString(const eWindowBackend& type) -> std::string {
    switch (type) {
        case eWindowBackend::TYPE_NONE:
            return "none";
        case eWindowBackend::TYPE_GLFW:
            return "glfw";
        case eWindowBackend::TYPE_EGL:
            return "egl";
        default:
            return "ERROR";
    }
}

}  // namespace renderer
