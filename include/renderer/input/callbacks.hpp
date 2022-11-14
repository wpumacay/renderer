#pragma once

#include <cstdint>
#include <functional>

namespace loco {
namespace renderer {

// Whether or not the shift keys were held down
constexpr int32_t MOD_SHIFT = (1 << 0);
// Whether or not the ctrl keys were held down
constexpr int32_t MOD_CONTROL = (1 << 1);
// Whether or not the alt keys were held down
constexpr int32_t MOD_ALT = (1 << 2);
// Whether or not the super key was held down
constexpr int32_t MOD_SUPER = (1 << 3);

using KeyboardCallback = std::function<void(int, int, int)>;
using MousePressCallback = std::function<void(int, int, double, double)>;
using MouseMoveCallback = std::function<void(double, double)>;
using ScrollCallback = std::function<void(double, double)>;
using ResizeCallback = std::function<void(int, int)>;

}  // namespace renderer
}  // namespace loco
