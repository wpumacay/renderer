#include <loco/renderer/window/impl/window_impl_glfw.hpp>
#include <loco/renderer/window/window.hpp>

namespace loco {
namespace renderer {

CWindow::CWindow(CWindowProperties properties)
    : m_Properties(std::move(properties)) {
    switch (m_Properties.backend) {
        case eWindowBackend::TYPE_GLFW:
            m_Impl = std::make_unique<WindowImplGlfw>(m_Properties);
            break;
        case eWindowBackend::TYPE_EGL:
            // TODO(wilbert): use WindowImplEgl here
            break;
        case eWindowBackend::TYPE_OSMESA:
            // TODO(wilbert): use WindowImplOsmesa here
            break;
        default:
            // TODO(wilbert): use WindowImplNone here
            break;
    }
}

auto CWindow::EnableCursor() -> void {
    if (m_Impl) {
        m_Impl->EnableCursor();
    }
}

auto CWindow::DisableCursor() -> void {
    if (m_Impl) {
        m_Impl->DisableCursor();
    }
}

auto CWindow::Begin() -> void {
    if (m_Impl) {
        m_Impl->Begin();
    }
}

auto CWindow::End() -> void {
    if (m_Impl) {
        m_Impl->End();
    }
}

auto CWindow::RequestClose() -> void {
    if (m_Impl) {
        m_Impl->RequestClose();
    }
}

auto CWindow::RegisterKeyboardCallback(const KeyboardCallback& callback)
    -> void {
    if (m_Impl) {
        m_Impl->RegisterKeyboardCallback(callback);
    }
}

auto CWindow::RegisterMousePressCallback(const MousePressCallback& callback)
    -> void {
    if (m_Impl) {
        m_Impl->RegisterMousePressCallback(callback);
    }
}

auto CWindow::RegisterMouseMoveCallback(const MouseMoveCallback& callback)
    -> void {
    if (m_Impl) {
        m_Impl->RegisterMouseMoveCallback(callback);
    }
}

auto CWindow::RegisterScrollCallback(const ScrollCallback& callback) -> void {
    if (m_Impl) {
        m_Impl->RegisterScrollCallback(callback);
    }
}

auto CWindow::RegisterResizeCallback(const ResizeCallback& callback) -> void {
    if (m_Impl) {
        m_Impl->RegisterResizeCallback(callback);
    }
}

}  // namespace renderer
}  // namespace loco
