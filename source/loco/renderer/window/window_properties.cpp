// clang-format off
#include <cassert>

#include <loco/renderer/window/window_properties.hpp>
// clang-format on

namespace loco {
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
}  // namespace loco
