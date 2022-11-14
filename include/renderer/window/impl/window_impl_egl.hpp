#pragma once

// clang-format off
#include <glad/gl.h>
#include <glad/egl.h>

#include <renderer/input/callbacks.hpp>
#include <renderer/window/impl/window_impl.hpp>
// clang-format on

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#endif

namespace loco {
namespace renderer {

class WindowImplEgl : public IWindowImpl {
    // cppcheck-suppress unknownMacro
    LOCO_NO_COPY_NO_MOVE_NO_ASSIGN(WindowImplEgl)

    LOCO_DEFINE_SMART_POINTERS(WindowImplEgl)

 public:
    explicit WindowImplEgl(WindowProperties properties);

    ~WindowImplEgl() override;

    // NOLINTNEXTLINE
    auto RegisterKeyboardCallback(const KeyboardCallback& callback)
        -> void override{/* Do nothing here */};

    // NOLINTNEXTLINE
    auto RegisterMousePressCallback(const MousePressCallback& callback)
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

    auto RequestClose() -> void override;

 private:
    EGLConfig m_Config = nullptr;
    EGLDisplay m_Display = nullptr;
    EGLSurface m_Surface = nullptr;
    EGLContext m_Context = nullptr;
};

}  // namespace renderer
}  // namespace loco

#ifdef __clang__
#pragma clang diagnostic pop  // NOLINT
#endif
