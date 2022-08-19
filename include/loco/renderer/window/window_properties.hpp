#pragma once

// clang-format off
#include <string>

#include <loco/renderer/common.hpp>
// clang-format on

namespace loco {
namespace renderer {

constexpr int DEFAULT_WINDOW_WIDTH = 1024;
constexpr int DEFAULT_WINDOW_HEIGHT = 768;
constexpr int MAX_CALLBACKS = 10;

enum class eWindowBackend {
    TYPE_NONE,
    TYPE_GLFW,
    TYPE_EGL,
    TYPE_OSMESA,
};

struct CWindowProperties {
    eWindowBackend backend = eWindowBackend::TYPE_NONE;
    int width = DEFAULT_WINDOW_WIDTH;
    int height = DEFAULT_WINDOW_HEIGHT;
    bool active = true;
    bool dirty_request_close = false;
    std::string title = "Application Window";
    CVec4 clear_color = {0.2F, 0.2F, 0.2F, 1.0F};  // NOLINT
    int gl_version_major = 3;
    int gl_version_minor = 3;
};

}  // namespace renderer
}  // namespace loco
