#include <renderer/input/input_manager_t.hpp>

namespace renderer {

auto InputManager::CallbackKey(int key, int action) -> void {
    m_Keys.at(static_cast<size_t>(key)) = action;
}

auto InputManager::CallbackMouseButton(int button, int action, float x, float y)
    -> void {
    m_Buttons.at(static_cast<size_t>(button)) = action;
    m_Cursor.x() = x;
    m_Cursor.y() = y;
}

auto InputManager::CallbackMouseMove(float x, float y) -> void {
    m_Cursor.x() = x;
    m_Cursor.y() = y;
}

auto InputManager::CallbackScroll(float xOff, float yOff) -> void {
    m_ScrollOff.x() = xOff;
    m_ScrollOff.y() = yOff;
    m_ScrollAccum.x() += xOff;
    m_ScrollAccum.y() += yOff;
}

auto InputManager::IsKeyDown(int key) const -> bool {
    if (key < 0 || key > MAX_KEYS) {
        return false;
    }

    return (m_Keys.at(static_cast<size_t>(key)) == key_action::PRESSED) ||
           (m_Keys.at(static_cast<size_t>(key)) == key_action::REPEAT);
}

auto InputManager::IsMouseDown(int button) const -> bool {
    if (button < 0 || button > MAX_BUTTONS) {
        return false;
    }

    return m_Buttons.at(static_cast<size_t>(button)) ==
           button_action::BUTTON_PRESSED;
}

}  // namespace renderer
