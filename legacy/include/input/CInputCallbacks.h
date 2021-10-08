#pragma once

#include <functional>

// args: key, action
typedef std::function<void(int,int)> FnPtr_keyboard_callback;
// args: button, action, x, y
typedef std::function<void(int,int,double,double)> FnPtr_mouse_callback;
// args: x, y
typedef std::function<void(double,double)> FnPtr_mousemove_callback;
// args: x, y
typedef std::function<void(double,double)> FnPtr_scroll_callback;
// args: width, height
typedef std::function<void(int,int)> FnPtr_resize_callback;
