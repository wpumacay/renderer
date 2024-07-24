#pragma once

#include <cstdint>

namespace renderer {

/// Maximum number of possible keycodes
constexpr uint16_t MAX_BUTTONS = 4;

namespace button_action {
enum ButtonAction {
    BUTTON_RELEASED = 0,  ///< Mouse button has been released
    BUTTON_PRESSED = 1    ///< Mouse button has been pressed
};
}

namespace mouse {
enum Mouse {
    BUTTON_LEFT = 0,   ///< Left button (left-click)
    BUTTON_RIGHT = 1,  ///< Right button (right-click)
    BUTTON_MIDDLE = 2  ///< Middle button (scrolling wheel)
};
}

}  // namespace renderer
