#pragma once

// Copied from cherno's hazel engine: 
//      https://github.com/TheCherno/Hazel/blob/55fc4392f4bb44d94a54f09522d5545a86dfc48a/Hazel/src/Hazel/MouseButtonCodes.h

namespace engine
{
    const int MAX_BUTTONS = 4;

    enum MouseAction
    {
        BUTTON_RELEASED = 0,
        BUTTON_PRESSED = 1
    };

    enum Mouse
    {
        BUTTON_LEFT   = 0,
        BUTTON_RIGHT  = 1,
        BUTTON_MIDDLE = 2
    };
}