#pragma once

// clang-format off
#include <array>
#include <string>
#include <loco/utils/common.hpp>

#include <loco/renderer/common.hpp>
#include <loco/renderer/input/callbacks.hpp>
#include <loco/renderer/window/window_properties.hpp>
#include <loco/renderer/window/impl/window_impl.hpp>
// clang-format on

namespace loco {
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
    LOCO_NO_COPY_NO_MOVE_NO_ASSIGN(Window)

    LOCO_DEFINE_SMART_POINTERS(Window)

 public:
    /// Creates a window from a given set of properties
    explicit Window(WindowProperties properties);

    /// Creates a window given its width, height, and (optionally) backend
    explicit Window(int width, int height,
                    const eWindowBackend& backend = eWindowBackend::TYPE_GLFW);

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

    /// Registers (if applicable) a callback to be called on keyboard events
    auto RegisterKeyboardCallback(const KeyboardCallback& callback) -> void;

    /// Registers (if applicable) a callback to be called on mouse-down events
    auto RegisterMousePressCallback(const MousePressCallback& callback) -> void;

    /// Registers (if applicable) a callback to be called on mouse-move events
    auto RegisterMouseMoveCallback(const MouseMoveCallback& callback) -> void;

    /// Registers (if applicable) a callback to called on mouse-scroll events
    auto RegisterScrollCallback(const ScrollCallback& callback) -> void;

    /// Registers (if applicable) a callback to be called on resize-window
    /// events
    auto RegisterResizeCallback(const ResizeCallback& callback) -> void;

    /// Returns the current width (in pixels) of this window
    auto width() const -> int { return m_Properties.width; }

    /// Returns the current height (in pixels) of this window
    auto height() const -> int { return m_Properties.height; }

    /// Returns whether or not this window is still active
    auto active() const -> bool { return m_Properties.active; }

    /// Returns the title of this window
    auto title() const -> std::string { return m_Properties.title; }

    /// Returns the identifier of the windowing-backend used for this window
    auto backend() const -> eWindowBackend { return m_Properties.backend; }

    /// Returns an unmutable reference to the properties of this window
    auto properties() const -> const WindowProperties& { return m_Properties; }

 private:
    auto _CreateImpl() -> void;

 private:
    WindowProperties m_Properties;
    /// Backend-specific implementation of the window API
    IWindowImpl::uptr m_Impl = nullptr;
};

}  // namespace renderer
}  // namespace loco
