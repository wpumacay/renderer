#pragma once

#include <glad/gl.h>
#include <glad/egl.h>

#include <renderer/input/callbacks.hpp>
#include <renderer/window/impl/window_impl.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

namespace renderer {

class WindowImplEgl : public IWindowImpl {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(WindowImplEgl)

    DEFINE_SMART_POINTERS(WindowImplEgl)

 public:
    explicit WindowImplEgl(WindowConfig config);

    ~WindowImplEgl() override;

    // NOLINTNEXTLINE
    auto RegisterKeyboardCallback(const KeyboardCallback& callback)
        -> void override{/* Do nothing here */};

    // NOLINTNEXTLINE
    auto RegisterMouseButtonCallback(const MouseButtonCallback& callback)
        -> void override{/* Do nothing here */};

    // NOLINTNEXTLINE
    auto RegisterMouseMoveCallback(const MouseMoveCallback& callback)
        -> void override{/* Do nothing here */};

    // NOLINTNEXTLINE
    auto RegisterScrollCallback(const ScrollCallback& callback)
        -> void override{/* Do nothing here */};

    // NOLINTNEXTLINE
    auto RegisterResizeCallback(const ResizeCallback& callback)
        -> void override{/* Do nothing here */};

    auto EnableCursor() -> void override{/* Do nothing here */};

    auto DisableCursor() -> void override{/* Do nothing here */};

    auto Begin() -> void override;

    auto End() -> void override;

    auto RequestClose() -> void override{/* Do nothing here */};

    auto SetClearColor(const Vec4& color) -> void override;

 private:
    EGLConfig m_EglConfig = nullptr;
    EGLDisplay m_EglDisplay = nullptr;
    EGLSurface m_EglSurface = nullptr;
    EGLContext m_EglContext = nullptr;
};

}  // namespace renderer

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
