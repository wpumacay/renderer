#pragma once

#include <string>
#include <iostream>

#include <utils/common.hpp>

#include <math/vec2_t.hpp>
#include <math/vec3_t.hpp>
#include <math/vec4_t.hpp>
#include <math/mat3_t.hpp>
#include <math/mat4_t.hpp>
#include <math/quat_t.hpp>
#include <math/euler_t.hpp>

using Vec2 = math::Vector2<float>;
using Vec3 = math::Vector3<float>;
using Vec4 = math::Vector4<float>;
using Mat3 = math::Matrix3<float>;
using Mat4 = math::Matrix4<float>;
using Quat = math::Quaternion<float>;
using Euler = math::Euler<float>;

using float32_t = math::float32_t;
using float64_t = math::float64_t;

static constexpr float PI = static_cast<float>(math::PI);

namespace renderer {

#if defined(RENDERER_RESOURCES_PATH)
// NOLINTNEXTLINE
static const std::string RESOURCES_PATH = RENDERER_RESOURCES_PATH;
#else
// NOLINTNEXTLINE
static const std::string RESOURCES_PATH = "../../resources/";
#endif

#if defined(RENDERER_EXAMPLES_PATH)
// NOLINTNEXTLINE
static const std::string EXAMPLES_PATH = RENDERER_EXAMPLES_PATH;
#else
// NOLINTNEXTLINE
static const std::string EXAMPLES_PATH = "../../examples/";
#endif

}  // namespace renderer
