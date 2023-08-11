#pragma once

#include <array>
#include <string>
#include <memory>

#include <GLFW/glfw3.h>

#include <renderer/input/callbacks.hpp>
#include <renderer/window/impl/window_impl.hpp>

namespace renderer {

struct GLFWwindowDeleter {
    auto operator()(GLFWwindow* ptr) const -> void;
};

class WindowImplGlfw : public IWindowImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(WindowImplGlfw)

    DEFINE_SMART_POINTERS(WindowImplGlfw)

 public:
    explicit WindowImplGlfw(WindowConfig config);

    ~WindowImplGlfw() override;

    auto RegisterKeyboardCallback(const KeyboardCallback& callback)
        -> void override;

    auto RegisterMouseButtonCallback(const MouseButtonCallback& callback)
        -> void override;

    auto RegisterMouseMoveCallback(const MouseMoveCallback& callback)
        -> void override;

    auto RegisterScrollCallback(const ScrollCallback& callback)
        -> void override;

    auto RegisterResizeCallback(const ResizeCallback& callback)
        -> void override;

    auto EnableCursor() -> void override;

    auto DisableCursor() -> void override;

    auto Begin() -> void override;

    auto End() -> void override;

    auto RequestClose() -> void override;

    auto SetClearColor(const Vec4& color) -> void override;

 private:
    std::unique_ptr<GLFWwindow, GLFWwindowDeleter> m_GlfwWindow = nullptr;

    std::array<KeyboardCallback, MAX_CALLBACKS> m_ArrKeyboardCallbacks;
    std::array<MouseButtonCallback, MAX_CALLBACKS> m_ArrMouseButtonCallbacks;
    std::array<MouseMoveCallback, MAX_CALLBACKS> m_ArrMouseMoveCallbacks;
    std::array<ScrollCallback, MAX_CALLBACKS> m_ArrScrollCallbacks;
    std::array<ResizeCallback, MAX_CALLBACKS> m_ArrResizeCallbacks;

    size_t m_ArrKeyboardCallbacksCount = 0;
    size_t m_ArrMouseButtonCallbacksCount = 0;
    size_t m_ArrMouseMoveCallbacksCount = 0;
    size_t m_ArrScrollCallbacksCount = 0;
    size_t m_ArrResizeCallbacksCount = 0;
};

}  // namespace renderer
