#pragma once

#include <memory>
#include <utility>

#include <renderer/common.hpp>
#include <renderer/engine/callbacks.hpp>
#include <renderer/engine/graphics/window_config_t.hpp>

namespace renderer {

class Window;

/// Interface for Window adapters, which link a Window object to its backend
class RENDERER_API IWindowAdapter {
    NO_COPY_NO_MOVE_NO_ASSIGN(IWindowAdapter)

    DEFINE_SMART_POINTERS(IWindowAdapter)

 public:
    explicit IWindowAdapter(WindowConfig w_conf)
        : m_Config(std::move(w_conf)) {}

    virtual ~IWindowAdapter() = default;

    auto SetWindowHandle(std::weak_ptr<Window> window_handle) -> void {
        m_WindowHandle = std::move(window_handle);
    }

    virtual auto RegisterKeyboardCallback(const KeyboardCallback& callback)
        -> void = 0;

    virtual auto RegisterMouseButtonCallback(
        const MouseButtonCallback& callback) -> void = 0;

    virtual auto RegisterMouseMoveCallback(const MouseMoveCallback& callback)
        -> void = 0;

    virtual auto RegisterScrollCallback(const ScrollCallback& callback)
        -> void = 0;

    virtual auto RegisterResizeCallback(const ResizeCallback& callback)
        -> void = 0;

    virtual auto EnableCursor() -> void = 0;

    virtual auto DisableCursor() -> void = 0;

    virtual auto Begin() -> void = 0;

    virtual auto End() -> void = 0;

    virtual auto RequestClose() -> void = 0;

    virtual auto SetClearColor(const Vec4& bg_color) -> void = 0;

 protected:
    /// The config struct used to create the related window
    WindowConfig m_Config;

    /// A non-owning reference to the window that uses this adapter
    std::weak_ptr<Window> m_WindowHandle;
};

}  // namespace renderer
