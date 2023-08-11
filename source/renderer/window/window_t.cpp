
#include <renderer/window/window_t.hpp>
#include <renderer/window/impl/window_impl_egl.hpp>
#include <renderer/window/impl/window_impl_glfw.hpp>

namespace renderer {

Window::Window(WindowConfig config) : m_Config(std::move(config)) {
    _CreateImpl();
}

Window::Window(int width, int height, eWindowBackend backend) {
    m_Config.width = width;
    m_Config.height = height;
    m_Config.backend = backend;
    _CreateImpl();
}

auto Window::_CreateImpl() -> void {
    switch (m_Config.backend) {
        case eWindowBackend::TYPE_GLFW:
            m_Impl = std::make_unique<WindowImplGlfw>(m_Config);
            break;
        case eWindowBackend::TYPE_EGL:
            m_Impl = std::make_unique<WindowImplEgl>(m_Config);
            break;
        default:
            // TODO(wilbert): use WindowImplNone here
            break;
    }
    m_Active = true;
}

auto Window::EnableCursor() -> void {
    if (m_Impl) {
        m_Impl->EnableCursor();
    }
}

auto Window::DisableCursor() -> void {
    if (m_Impl) {
        m_Impl->DisableCursor();
    }
}

auto Window::Begin() -> void {
    if (m_Impl) {
        m_Impl->Begin();
    }
}

auto Window::End() -> void {
    if (m_Impl) {
        m_Impl->End();
    }
}

auto Window::RequestClose() -> void {
    if (m_Impl) {
        m_Impl->RequestClose();
    }
    m_Active = false;
}

auto Window::SetClearColor(const Vec4& color) -> void {
    if (m_Impl) {
        m_Impl->SetClearColor(color);
    }
    m_Config.clear_color = color;
}

auto Window::RegisterKeyboardCallback(const KeyboardCallback& callback)
    -> void {
    if (m_Impl) {
        m_Impl->RegisterKeyboardCallback(callback);
    }
}

auto Window::RegisterMouseButtonCallback(const MouseButtonCallback& callback)
    -> void {
    if (m_Impl) {
        m_Impl->RegisterMouseButtonCallback(callback);
    }
}

auto Window::RegisterMouseMoveCallback(const MouseMoveCallback& callback)
    -> void {
    if (m_Impl) {
        m_Impl->RegisterMouseMoveCallback(callback);
    }
}

auto Window::RegisterScrollCallback(const ScrollCallback& callback) -> void {
    if (m_Impl) {
        m_Impl->RegisterScrollCallback(callback);
    }
}

auto Window::RegisterResizeCallback(const ResizeCallback& callback) -> void {
    if (m_Impl) {
        m_Impl->RegisterResizeCallback(callback);
    }
}

}  // namespace renderer
