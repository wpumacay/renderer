#include <cassert>
#include <renderer/window/window_config_t.hpp>

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
            assert(false);
            return "ERROR";
    }
}

}  // namespace renderer
