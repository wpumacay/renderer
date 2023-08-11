#pragma once

#include <array>
#include <string>
#include <utils/common.hpp>

#include <renderer/common.hpp>
#include <renderer/input/callbacks.hpp>
#include <renderer/input/keycodes.hpp>
#include <renderer/window/window_config_t.hpp>
#include <renderer/window/impl/window_impl.hpp>

namespace renderer {

/// Abstraction of a window supporting multiple windowing backends
///
/// This class abstracts away the functionality required for a single
/// window, where the user can render stuff into (holds the framebuffer).
/// Note that the internal windowing backend used can vary, as the impl used
/// is polymorphic and can handle various backends, as long as the
/// implementation sticks to the public API exposed by this class
class Window {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Window)

    DEFINE_SMART_POINTERS(Window)

 public:
    /// Creates a window from a given set of configuration options
    explicit Window(WindowConfig config);

    /// Creates a window given its width, height, and (optionally) backend
    explicit Window(int width, int height,
                    eWindowBackend backend = eWindowBackend::TYPE_GLFW);

    ~Window() = default;

    /// Requests (if applicable) enabling the cursor on this window
    auto EnableCursor() -> void;

    /// Requests (if applicable) disabling the cursor on this window
    auto DisableCursor() -> void;

    /// Prepares the window for receiving rendering commands
    auto Begin() -> void;

    /// Cleans up and setup the window after all rendering has been done
    auto End() -> void;

    /// Requests (if applicable) the window to close itself in the next frame
    auto RequestClose() -> void;

    /// Sets the background color of the window
    auto SetClearColor(const Vec4& color) -> void;

    /// Registers (if applicable) a callback to be called on keyboard events
    auto RegisterKeyboardCallback(const KeyboardCallback& callback) -> void;

    /// Registers (if applicable) a callback to be called on mouse-down events
    auto RegisterMouseButtonCallback(const MouseButtonCallback& callback)
        -> void;

    /// Registers (if applicable) a callback to be called on mouse-move events
    auto RegisterMouseMoveCallback(const MouseMoveCallback& callback) -> void;

    /// Registers (if applicable) a callback to called on mouse-scroll events
    auto RegisterScrollCallback(const ScrollCallback& callback) -> void;

    /// Registers (if applicable) a callback to be called on resize-window
    /// events
    auto RegisterResizeCallback(const ResizeCallback& callback) -> void;

    /// Returns whether or not this window is still active
    auto active() const -> bool { return m_Active; }

    /// Returns an unmutable reference to the clear-color property
    auto clear_color() const -> Vec4 { return m_Config.clear_color; }

    /// Returns the current width (in pixels) of this window
    auto width() const -> int { return m_Config.width; }

    /// Returns the current height (in pixels) of this window
    auto height() const -> int { return m_Config.height; }

    /// Returns the title of this window
    auto title() const -> std::string { return m_Config.title; }

    /// Returns the identifier of the windowing-backend used for this window
    auto backend() const -> eWindowBackend { return m_Config.backend; }

    /// Returns an unmutable reference to the config of this window
    auto config() const -> const WindowConfig& { return m_Config; }

 private:
    auto _CreateImpl() -> void;

 private:
    /// The configuration used for creating this window
    WindowConfig m_Config;
    /// Whether or not the window is still active
    bool m_Active = false;
    /// Backend-specific implementation of the window API
    IWindowImpl::uptr m_Impl = nullptr;
};

}  // namespace renderer
