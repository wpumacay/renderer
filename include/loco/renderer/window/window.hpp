#pragma once

// clang-format off
#include <array>
#include <string>
#include <loco/utils/common.hpp>

#include <loco/renderer/common.hpp>
#include <loco/renderer/input/callbacks.hpp>
#include <loco/renderer/window/window_properties.hpp>
#include <loco/renderer/window/impl/window_impl.hpp>
// clang-format on

namespace loco {
namespace renderer {

class CWindow {
    LOCO_NO_COPY_NO_MOVE_NO_ASSIGN(CWindow)

 public:
    ~CWindow() = default;

    auto EnableCursor() -> void;

    auto DisableCursor() -> void;

    auto Begin() -> void;

    auto End() -> void;

    auto RequestClose() -> void;

    auto RegisterKeyboardCallback(const KeyboardCallback& callback) -> void;

    auto RegisterMousePressCallback(const MousePressCallback& callback) -> void;

    auto RegisterMouseMoveCallback(const MouseMoveCallback& callback) -> void;

    auto RegisterScrollCallback(const ScrollCallback& callback) -> void;

    auto RegisterResizeCallback(const ResizeCallback& callback) -> void;

    auto width() const -> int { return m_Properties.width; }

    auto height() const -> int { return m_Properties.height; }

    auto active() const -> bool { return m_Properties.active; }

    auto title() const -> std::string { return m_Properties.title; }

    auto backend() const -> eWindowBackend { return m_Properties.backend; }

 private:
    CWindowProperties m_Properties;
    /// Backend-specific implementation of the window API
    IWindowImpl::uptr m_Impl = nullptr;
};

}  // namespace renderer
}  // namespace loco
