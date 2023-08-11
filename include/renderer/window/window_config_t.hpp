#pragma once

#include <string>
#include <renderer/common.hpp>

namespace renderer {

/// Default width of the window
constexpr int DEFAULT_WINDOW_WIDTH = 1024;
/// Default height of the window
constexpr int DEFAULT_WINDOW_HEIGHT = 768;
/// Size of the container used to store window callbacks
constexpr int MAX_CALLBACKS = 10;

/// Available windowing backends
enum class eWindowBackend {
    /// None or dummy backend (for testing purposes only)
    TYPE_NONE,
    /// GLFW backend (used for creating a window for OpenGL in any platform)
    TYPE_GLFW,
    /// EGL backend (used for headless rendering mode)
    TYPE_EGL,
};

/// Returns the string representation of the given backend type
auto ToString(const eWindowBackend& type) -> std::string;

/// Configuration options for our window object
struct WindowConfig {
    /// Backend used for creating an OpenGL context
    eWindowBackend backend = eWindowBackend::TYPE_NONE;
    /// Desired width of the window
    int width = DEFAULT_WINDOW_WIDTH;
    /// Desired height of the window
    int height = DEFAULT_WINDOW_HEIGHT;
    /// Title of the window
    std::string title = "Application Window";
    /// Color used for the background of the window
    Vec4 clear_color = {0.2F, 0.2F, 0.2F, 1.0F};  // NOLINT
    /// OpenGL version major
    int gl_version_major = 3;
    /// OpenGL version minor
    int gl_version_minor = 3;
};

}  // namespace renderer
