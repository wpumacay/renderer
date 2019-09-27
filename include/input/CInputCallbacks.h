#pragma once

typedef void ( *FnPtr_keyboard_callback )( int key, int action );
typedef void ( *FnPtr_mouse_callback )( int button, int action, double x, double y );
typedef void ( *FnPtr_mousemove_callback )( double x, double y );
typedef void ( *FnPtr_scroll_callback )( double xOff, double yOff );
typedef void ( *FnPtr_resize_callback )( int width, int height );