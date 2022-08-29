#pragma once

// clang-format off
#include <string>
#include <iostream>

#include <loco/math/vec2_t_impl.hpp>
#include <loco/math/vec3_t_impl.hpp>
#include <loco/math/vec4_t_impl.hpp>
#include <loco/math/mat4_t_impl.hpp>
// clang-format on

using Vec2 = loco::math::Vector2<float>;
using Vec3 = loco::math::Vector3<float>;
using Vec4 = loco::math::Vector4<float>;
using Mat4 = loco::math::Matrix4<float>;

using float32_t = loco::math::float32_t;
using float64_t = loco::math::float64_t;

namespace loco {
namespace renderer {

/// Data types available for storing a single pixel (e.g. in a framebuffer)
enum class ePixelDataType {
    UINT_8,     ///< Pixel value with 1-byte of storage
    UINT_32,    ///< Pixel value with 4-bytes of storage
    UINT_24_8,  ///< Pixel value with 3-bytes + 1-bytes of (separated)
                ///< storage
    FLOAT_32,   ///< Pixel value with 4-bytes of storage as a 32-bit float
};

/// Returns the string representation of a given ePixelDataType
/// \param dtype Pixel data type storage representation
/// \return String representation of the given data type
auto ToString(const ePixelDataType& dtype) -> std::string;

/// Pipes the value of the given pixel data type representation into the
/// given buffer, and returns the buffer for further piping if required
/// \param[in,out] out_stream The output stream to pipe our dtype into
/// \param[in] dtype The data-type representation we want to pipe
/// \return The output stream after piping the given dtype
auto operator<<(std::ostream& out_stream, const ePixelDataType& dtype)
    -> std::ostream&;

}  // namespace renderer
}  // namespace loco
