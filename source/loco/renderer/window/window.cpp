#include <loco/renderer/window/window.hpp>

namespace loco {
namespace renderer {

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
