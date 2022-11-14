#pragma once

#include <string>
#include <iostream>

#include <loco/utils/common.hpp>

#include <math/vec2_t.hpp>
#include <math/vec3_t.hpp>
#include <math/vec4_t.hpp>
#include <math/mat4_t.hpp>

using Vec2 = math::Vector2<float>;
using Vec3 = math::Vector3<float>;
using Vec4 = math::Vector4<float>;
using Mat4 = math::Matrix4<float>;

using float32_t = math::float32_t;
using float64_t = math::float64_t;

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
