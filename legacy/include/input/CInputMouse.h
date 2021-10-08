/// @file CInputMouse.h
/// @brief Mouse-related definitions
#pragma once

// Adapted from Cherno's hazel engine:
//      https://github.com/TheCherno/Hazel/blob/55fc4392f4bb44d94a54f09522d5545a86dfc48a/Hazel/src/Hazel/MouseButtonCodes.h

namespace engine
{
    /// Maximum number of mouse-buttons possible (unless gamer mouse)
    constexpr ssize_t MAX_BUTTONS = 4;

    /// States in which the mouse buttons can be
    enum MouseAction
    {
        BUTTON_RELEASED = 0, ///< Mouse button has been release
        BUTTON_PRESSED = 1   ///< Mouse button has been pressed
    };

    /// Available mouse buttons
    enum Mouse
    {
        BUTTON_LEFT   = 0, ///< Left button (left-click)
        BUTTON_RIGHT  = 1, ///< Right button (right-click)
        BUTTON_MIDDLE = 2  ///< Middle button (scrolling wheel)
    };
}
