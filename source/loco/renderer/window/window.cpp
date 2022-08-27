// clang-format off
#include <loco/renderer/window/window.hpp>
#include <loco/renderer/window/impl/window_impl_egl.hpp>
#include <loco/renderer/window/impl/window_impl_glfw.hpp>
// clang-format on

namespace loco {
namespace renderer {

Window::Window(WindowProperties properties)
    : m_Properties(std::move(properties)) {
    _CreateImpl();
}

Window::Window(int width, int height, const eWindowBackend& backend) {
    m_Properties.width = width;
    m_Properties.height = height;
    m_Properties.backend = backend;
    _CreateImpl();
}

auto Window::_CreateImpl() -> void {
    switch (m_Properties.backend) {
        case eWindowBackend::TYPE_GLFW:
            m_Impl = std::make_unique<WindowImplGlfw>(m_Properties);
            break;
        case eWindowBackend::TYPE_EGL:
            m_Impl = std::make_unique<WindowImplEgl>(m_Properties);
            break;
        case eWindowBackend::TYPE_OSMESA:
            // TODO(wilbert): use WindowImplOsmesa here
            break;
        default:
            // TODO(wilbert): use WindowImplNone here
            break;
    }
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
}

auto Window::RegisterKeyboardCallback(const KeyboardCallback& callback)
    -> void {
    if (m_Impl) {
        m_Impl->RegisterKeyboardCallback(callback);
    }
}

auto Window::RegisterMousePressCallback(const MousePressCallback& callback)
    -> void {
    if (m_Impl) {
        m_Impl->RegisterMousePressCallback(callback);
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
}  // namespace loco
