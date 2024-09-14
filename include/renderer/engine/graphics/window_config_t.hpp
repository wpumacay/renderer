#pragma once

#include <string>

#include <renderer/common.hpp>
#include <renderer/engine/graphics/enums.hpp>

namespace renderer {

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
