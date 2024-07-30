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
#include <math/pose3d_t.hpp>

using Vec2 = math::Vector2<float>;
using Vec3 = math::Vector3<float>;
using Vec4 = math::Vector4<float>;
using Mat3 = math::Matrix3<float>;
using Mat4 = math::Matrix4<float>;
using Quat = math::Quaternion<float>;
using Euler = math::Euler<float>;
using Pose = math::Pose3d<float>;

using float32_t = math::float32_t;
using float64_t = math::float64_t;

static constexpr float PI = static_cast<float>(math::PI);

// -----------------------------------------------------------------------------
// Language detection adapted from https://github.com/g-truc/glm

#define RENDERER_LANG_CXX98_FLAG (1 << 1)
#define RENDERER_LANG_CXX03_FLAG (1 << 2)
#define RENDERER_LANG_CXX0X_FLAG (1 << 3)
#define RENDERER_LANG_CXX11_FLAG (1 << 4)
#define RENDERER_LANG_CXX14_FLAG (1 << 5)
#define RENDERER_LANG_CXX17_FLAG (1 << 6)
#define RENDERER_LANG_CXX20_FLAG (1 << 7)

#define RENDERER_LANG_CXX98 RENDERER_LANG_CXX98_FLAG
#define RENDERER_LANG_CXX03 (RENDERER_LANG_CXX98 | RENDERER_LANG_CXX03_FLAG)
#define RENDERER_LANG_CXX0X (RENDERER_LANG_CXX03 | RENDERER_LANG_CXX0X_FLAG)
#define RENDERER_LANG_CXX11 (RENDERER_LANG_CXX0X | RENDERER_LANG_CXX11_FLAG)
#define RENDERER_LANG_CXX14 (RENDERER_LANG_CXX11 | RENDERER_LANG_CXX14_FLAG)
#define RENDERER_LANG_CXX17 (RENDERER_LANG_CXX14 | RENDERER_LANG_CXX17_FLAG)
#define RENDERER_LANG_CXX20 (RENDERER_LANG_CXX17 | RENDERER_LANG_CXX20_FLAG)

// clang-format off
#if defined(RENDERER_FORCE_CXX20)
    #define RENDERER_LANG RENDERER_LANG_CXX20
#elif defined(RENDERER_FORCE_CXX17)
    #define RENDERER_LANG RENDERER_LANG_CXX17
#elif defined(RENDERER_FORCE_CXX14)
    #define RENDERER_LANG RENDERER_LANG_CXX14
#elif defined(RENDERER_FORCE_CXX11)
    #define RENDERER_LANG RENDERER_LANG_CXX11
#else
    #if __cplusplus > 201703L
        #define RENDERER_LANG RENDERER_LANG_CXX20
    #elif __cplusplus == 201703L
        #define RENDERER_LANG RENDERER_LANG_CXX17
    #elif __cplusplus == 201402L
        #define RENDERER_LANG RENDERER_LANG_CXX14
    #elif __cplusplus == 201103L
        #define RENDERER_LANG RENDERER_LANG_CXX11
    #else
        #error "C++ standard must be one of 11, 14, 17, and 20"
    #endif
#endif

// [[nodiscard]]
#if RENDERER_LANG & RENDERER_LANG_CXX17_FLAG
    #define RENDERER_NODISCARD [[nodiscard]]
#else
    #define RENDERER_NODISCARD
#endif

#if defined _WIN32 || defined __CYGWIN__
    #define RENDERER_DLL_EXPORT __declspec(dllexport)
    #define RENDERER_DLL_IMPORT __declspec(dllimport)
    #define RENDERER_DLL_LOCAL
#else
    #if __GNUC__ >= 4
        #define RENDERER_DLL_EXPORT __attribute__ ((visibility ("default")))
        #define RENDERER_DLL_IMPORT __attribute__ ((visibility ("default")))
        #define RENDERER_DLL_LOCAL __attribute__ ((visibility ("hidden")))
    #else
        #define RENDERER_DLL_EXPORT
        #define RENDERER_DLL_IMPORT
        #define RENDERER_DLL_LOCAL
    #endif
#endif


#define RENDERER_DECL RENDERER_NODISCARD

#ifdef RENDERER_STATIC
    #define RENDERER_API
    #define RENDERER_LOCAL
#else
    #ifdef RENDERER_DLL_EXPORTS
        #define RENDERER_API RENDERER_DLL_EXPORT
    #else
        #define RENDERER_API RENDERER_DLL_IMPORT
    #endif
    #define RENDERER_LOCAL RENDERER_DLL_LOCAL
#endif
// clang-format on

// -----------------------------------------------------------------------------

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
