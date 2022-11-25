#pragma once

#include <renderer/geometry/geometry_t.hpp>

namespace renderer {

/// Axis used define the main axis when creating some geometry types
enum class eAxis {
    AXIS_X,  ///< Main axis is the X axis
    AXIS_Y,  ///< Main axis is the Y axis
    AXIS_Z   ///< Main axis is the Z axis
};

/// Creates the geometry for a plane given the width and depth
/// \param[in] width The width of the plane (x-dimension)
/// \param[in] depth The depth of the plane (y-dimension)
/// \param[in] axis The axis for the normal to the plane
auto CreatePlane(float width, float depth, const eAxis& axis) -> Geometry::uptr;

/// Creates the geometry for a box its dimensions along the x, y, z axes
/// \param[in] width The width of the box (x-dimension)
/// \param[in] depth The depth of the box (y-dimension)
/// \param[in] height The height of the box (z-dimension)
auto CreateBox(float width, float depth, float height) -> Geometry::uptr;

auto CreateSphere(float radius, size_t nDiv1 = 20, size_t nDiv2 = 20)
    -> Geometry::uptr;

auto CreateEllipsoid(float radiusX, float radiusY, float radiusZ,
                     size_t nDiv1 = 20, size_t nDiv2 = 20) -> Geometry::uptr;

auto CreateCylinder(float radius, float height,
                    const eAxis& axis = eAxis::AXIS_Z, size_t nDiv1 = 30)
    -> Geometry::uptr;

auto CreateCapsule(float radius, float height,
                   const eAxis& axis = eAxis::AXIS_Z, size_t nDiv1 = 30,
                   size_t nDiv2 = 30) -> Geometry::uptr;

auto CreateArrow(float length, const eAxis& axis = eAxis::AXIS_Z)
    -> Geometry::uptr;

auto CreateAxes(float length) -> Geometry::uptr;

auto _RotateToMatchUpAxis(const Vec3& vec, const eAxis& axis) -> Vec3;

}  // namespace renderer
