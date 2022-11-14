#pragma once

#include <utility>

#include <loco/utils/common.hpp>
#include <renderer/common.hpp>
#include <renderer/input/callbacks.hpp>
#include <renderer/window/window_properties.hpp>

namespace renderer {

class Window;

class IWindowImpl {
    LOCO_NO_COPY_NO_MOVE_NO_ASSIGN(IWindowImpl);

    LOCO_DEFINE_SMART_POINTERS(IWindowImpl);

 public:
    explicit IWindowImpl(WindowProperties properties)
        : m_Properties(std::move(properties)) {}

    virtual ~IWindowImpl() = default;

    virtual auto RegisterKeyboardCallback(const KeyboardCallback& callback)
        -> void = 0;

    virtual auto RegisterMousePressCallback(const MousePressCallback& callback)
        -> void = 0;

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

 protected:
    WindowProperties m_Properties;
};

}  // namespace renderer
