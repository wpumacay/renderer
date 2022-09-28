#pragma once

#include <string>
#include <iostream>

#include <loco/utils/common.hpp>

#include <loco/math/vec2_t_impl.hpp>
#include <loco/math/vec3_t_impl.hpp>
#include <loco/math/vec4_t_impl.hpp>
#include <loco/math/mat4_t_impl.hpp>

using Vec2 = loco::math::Vector2<float>;
using Vec3 = loco::math::Vector3<float>;
using Vec4 = loco::math::Vector4<float>;
using Mat4 = loco::math::Matrix4<float>;

using float32_t = loco::math::float32_t;
using float64_t = loco::math::float64_t;

namespace loco {
namespace renderer {

#if defined(LOCORENDERER_RESOURCES_PATH)
// NOLINTNEXTLINE
static const std::string RESOURCES_PATH = LOCORENDERER_RESOURCES_PATH;
#else
// NOLINTNEXTLINE
static const std::string RESOURCES_PATH = "../../resources/";
#endif

#if defined(LOCORENDERER_EXAMPLES_PATH)
// NOLINTNEXTLINE
static const std::string EXAMPLES_PATH = LOCORENDERER_EXAMPLES_PATH;
#else
// NOLINTNEXTLINE
static const std::string EXAMPLES_PATH = "../../examples/";
#endif

}  // namespace renderer
}  // namespace loco
