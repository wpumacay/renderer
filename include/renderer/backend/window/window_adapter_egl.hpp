#pragma once

#include <glad/egl.h>

#include <renderer/engine/callbacks.hpp>
#include <renderer/engine/graphics/window_adapter_t.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

namespace renderer {

class RENDERER_API WindowAdapterEGL : public IWindowAdapter {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(WindowAdapterEGL)

    DEFINE_SMART_POINTERS(WindowAdapterEGL)

 public:
    explicit WindowAdapterEGL(WindowConfig config);

    ~WindowAdapterEGL() override;

    auto RegisterKeyboardCallback(const KeyboardCallback& callback)
        -> void override{/* Do nothing here */};

    auto RegisterMouseButtonCallback(const MouseButtonCallback& callback)
        -> void override{/* Do nothing here */};

    auto RegisterMouseMoveCallback(const MouseMoveCallback& callback)
        -> void override{/* Do nothing here */};

    auto RegisterScrollCallback(const ScrollCallback& callback)
        -> void override{/* Do nothing here */};

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
