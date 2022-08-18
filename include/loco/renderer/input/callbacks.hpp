#pragma once

#include <functional>

using KeyboardCallback = std::function<void(int, int)>;
using MousePressCallback = std::function<void(int, int, double, double)>;
using MouseMoveCallback = std::function<void(double, double)>;
using ScrollCallback = std::function<void(double, double)>;
using ResizeCallback = std::function<void(int, int)>;
