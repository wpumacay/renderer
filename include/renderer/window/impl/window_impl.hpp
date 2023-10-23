#pragma once

#include <utility>

#include <utils/common.hpp>
#include <renderer/common.hpp>
#include <renderer/input/callbacks.hpp>
#include <renderer/window/window_config_t.hpp>

namespace renderer {

class Window;

class IWindowImpl {
    NO_COPY_NO_MOVE_NO_ASSIGN(IWindowImpl)

    DEFINE_SMART_POINTERS(IWindowImpl)

 public:
    explicit IWindowImpl(WindowConfig config) : m_Config(std::move(config)) {}

    virtual ~IWindowImpl() = default;

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

    virtual auto SetClearColor(const Vec4& color) -> void = 0;

 protected:
    WindowConfig m_Config;
};

}  // namespace renderer
