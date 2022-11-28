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

/// Creates the geometry for a box given the dimensions along the x, y, z axes
/// \param[in] width The width of the box (x-dimension)
/// \param[in] depth The depth of the box (y-dimension)
/// \param[in] height The height of the box (z-dimension)
auto CreateBox(float width, float depth, float height) -> Geometry::uptr;

/// Creates the geometry for a sphere given its radius and tessellation level
/// \param[in] radius The radius of the sphere
/// \param[in] nDiv1 The tessellation level for the second spherical dimension
/// \param[in] nDiv2 The tessellation level for the third spherical dimension
auto CreateSphere(float radius, size_t nDiv1 = 20, size_t nDiv2 = 20)
    -> Geometry::uptr;

/// Creates the geometry for an ellipsoid given its size and tessellation level
/// \param[in] radius_x The radius in the x-axis of the ellipsoid
/// \param[in] radius_y The radius in the y-axis of the ellipsoid
/// \param[in] radius_z The radius in the z-axis of the ellipsoid
/// \param[in] nDiv1 The tessellation level for the second spherical dimension
/// \param[in] nDiv2 The tessellation level for the third spherical dimensions
auto CreateEllipsoid(float radius_x, float radius_y, float radius_z,
                     size_t nDiv1 = 20, size_t nDiv2 = 20) -> Geometry::uptr;

/// Creates the geometry for a cylinder given its size and tessellation level
/// \param[in] radius Radius of the bases of the cylinder
/// \param[in] height Height of the cylinder
/// \param[in] axis Direction of the principal axis of the cylinder
/// \param[in] nDiv The tessellation level for the body of the cylinder
auto CreateCylinder(float radius, float height,
                    const eAxis& axis = eAxis::AXIS_Z, size_t nDiv = 30)
    -> Geometry::uptr;

/// Creates the geometry for a capsule given its size and tessellation level
/// \param[in] radius Radius of the section and both caps
/// \param[in] height Height of the capsule (without the caps)
/// \param[in] axis Direction of the principal axis of the capsule
/// \param[in] nDiv1 The tessellation level of the cylindrical part
/// \param[in] nDiv2 The tesellation level for the caps of the capsule
auto CreateCapsule(float radius, float height,
                   const eAxis& axis = eAxis::AXIS_Z, size_t nDiv1 = 30,
                   size_t nDiv2 = 30) -> Geometry::uptr;

/// Creates the geometry for an arrow given its size and main axis
/// \param[in] length The length of the arrow
/// \param[in] axis The main axis of the arrow (direction it points to)
auto CreateArrow(float length, const eAxis& axis = eAxis::AXIS_Z)
    -> Geometry::uptr;

auto _RotateToMatchUpAxis(const Vec3& vec, const eAxis& axis) -> Vec3;

}  // namespace renderer
