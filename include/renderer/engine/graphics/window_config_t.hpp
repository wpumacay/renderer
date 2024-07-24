#pragma once

#include <string>
#include <renderer/common.hpp>

namespace renderer {

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

/// Default windowing backend
constexpr auto DEFAULT_WINDOWING_BACKEND = eWindowBackend::TYPE_NONE;
/// Default width of the window
constexpr int DEFAULT_WINDOW_WIDTH = 1024;
/// Default height of the window
constexpr int DEFAULT_WINDOW_HEIGHT = 768;
/// Default window title
constexpr auto DEFAULT_WINDOW_TITLE = "Application Window";
/// Default OpenGL Major Version
constexpr int DEFAULT_OPENGL_MAJOR_VERSION = 3;
/// Default OpenGL Minor Version
constexpr int DEFAULT_OPENGL_MINOR_VERSION = 3;
/// Size of the container used to store window callbacks
constexpr int MAX_CALLBACKS = 10;

/// Configuration options for our window object
struct WindowConfig {
    /// Backend used for creating an OpenGL context
    eWindowBackend backend = DEFAULT_WINDOWING_BACKEND;
    /// Desired width of the window
    int width = DEFAULT_WINDOW_WIDTH;
    /// Desired height of the window
    int height = DEFAULT_WINDOW_HEIGHT;
    /// Title of the window
    std::string title = DEFAULT_WINDOW_TITLE;
    /// Color used for the background of the window
    Vec4 clear_color = {0.2F, 0.2F, 0.2F, 1.0F};  // NOLINT
    /// OpenGL version major
    int gl_version_major = DEFAULT_OPENGL_MAJOR_VERSION;
    /// OpenGL version minor
    int gl_version_minor = DEFAULT_OPENGL_MINOR_VERSION;
};

}  // namespace renderer
