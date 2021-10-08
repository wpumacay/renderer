#pragma once

#include <cstdint>
#include <algorithm>
// tinymath-library
#include <vector_t.h>
#include <matrix_t.h>
#include <transforms.h>

namespace engine
{
    typedef uint8_t uint8;
    typedef uint32_t uint32;
    typedef uint64_t uint64;
    typedef int32_t int32;
    typedef int64_t int64;
    typedef float float32;
    typedef double float64;

    constexpr float32 PI = 3.141592653589793f;
    constexpr float32 EPS = 1e-6f;

    // Redefine the vector types to be use in the codebase
    typedef tinymath::Vector<float32, 2 > CVec2;
    typedef tinymath::Vector<float32, 3 > CVec3;
    typedef tinymath::Vector<float32, 4 > CVec4;

    // Redefine the matrix types to use in the codebase
    typedef tinymath::Matrix<float32, 2 > CMat2;
    typedef tinymath::Matrix<float32, 3 > CMat3;
    typedef tinymath::Matrix<float32, 4 > CMat4;

    // Define our own to-string methods (mostly to avoid replacing current engine::ToString usage)
    std::string ToString( const CVec2& v );
    std::string ToString( const CVec3& v );
    std::string ToString( const CVec4& v );
    std::string ToString( const CMat2& m );
    std::string ToString( const CMat3& m );
    std::string ToString( const CMat4& m );

    //--------------------------------------------------------------------------------------------//
    //                                     Matrix4x4 helpers                                      //
    //--------------------------------------------------------------------------------------------//

    /// Returns a perspective-projection matrix constructed with the given parameters
    ///
    /// @param fov Field of view of for perspective matrix
    /// @param aspect Aspect ratio of for perspective matrix
    /// @param zNear Near value in the z-direction of the view-frustum of the perspective matrix
    /// @param zFar Far value in the z-direction of the view-frustum of the perspective matrix
    /// @return Perspective-projection matrix with the given configuration
    CMat4 Perspective( float32 fov, float32 aspect, float32 zNear, float32 zFar );

    /// Returns an orthographic-projectino matrix constructed with the given parameters
    ///
    /// @param width Width of the view-frustum (view-box) for the ortographic matrix
    /// @param height Height of the view-frustum (view-box) for the ortographic matrix
    /// @param zNear Near value in the z-direction of the view-frustum (view-box) of the ortographic matrix
    /// @param zFar Far value in the z-direction of the view-frustum (view-box) of the ortographic matrix
    /// @return Ortographic-projection matrix with the given configuration
    CMat4 Ortho( float32 width, float32 height, float32 zNear, float32 zFar );

    /// Returns a view-matrix for a camera with given position and target point
    ///
    /// @param position Origin of the frame of reference of the camera
    /// @param target Target point the camera looks at
    /// @param worldUp Up-vector of the world reference frame
    /// @return View matrix with the given configuration
    CMat4 LookAt( const CVec3& position, const CVec3& target, const CVec3& worldUp );

    /// Returns a 4x4 homogeneous scale matrix with the given scale parameters
    ///
    /// @param scales Vector with x-y-z scale values
    /// @return Homogeneous 4x4 scale matrix corresponding to the given scale
    CMat4 Scale( const CVec3& scales );

    /// Returns a 4x4 homogeneous rotation matrix with the given rotation parameters
    ///
    /// @param rot 3x3 rotation matrix describing the rotation
    /// @return Homogeneous 4x4 rotation matrix corresponding to the given rotation
    CMat4 Rotation( const CMat3& rot );

    /// Returns a 4x4 homogeneous translation matrix with the given translation parameters
    ///
    /// @param pos Position vector used to describe the translation
    /// @return Homogeneous 4x4 translation matrix corresponding to the given translation
    CMat4 Translation( const CVec3& pos );

    /// Returns a 4x4 homogeneous rotation matrix for a given rotation around the x-axis
    ///
    /// @param angle Rotation angle around the x-axis used to describe the rotation
    /// @return Homogeneous 4x4 rotation matrix corresponding to a rotation around the x-axis
    CMat4 RotationX( float32 angle );

    /// Returns a 4x4 homogeneous rotation matrix for a given rotation around the y-axis
    ///
    /// @param angle Rotation angle around the y-axis used to describe the rotation
    /// @return Homogeneous 4x4 rotation matrix corresponding to a rotation around the y-axis
    CMat4 RotationY( float32 angle );

    /// Returns a 4x4 homogeneous rotation matrix for a given rotation around the z-axis
    ///
    /// @param angle Rotation angle around the z-axis used to describe the rotation
    /// @return Homogeneous 4x4 rotation matrix corresponding to a rotation around the z-axis
    CMat4 RotationZ( float32 angle );

    //--------------------------------------------------------------------------------------------//
    //                                            CInd3                                           //
    //--------------------------------------------------------------------------------------------//

    /// Vector-3 with integer components, used to represent faces and index data
    struct CInd3
    {
        /// Entries of the integer vector-3
        int32 buff[3];
        /// Constructs a default vector-3 with all integer entries set to zero
        CInd3();
        /// Constructs a vector-3 with the given integer entries
        CInd3( int32 v1, int32 v2, int32 v3 );
    };

    /// Returns a string representation of the integer vector-3
    ///
    /// @param ind3 Vector-3 with integer entries
    /// @return String representation of the given vector
    std::string ToString( const CInd3& ind3 );

    //--------------------------------------------------------------------------------------------//
    //                                      Geometric helpers                                     //
    //--------------------------------------------------------------------------------------------//

    /// Line object, represented as a start and end point
    struct CLine
    {
        /// Start point of the line
        CVec3 start = { 0.0f, 0.0f, 0.0f };
        /// End point of the line
        CVec3 end = { 0.0f, 0.0f, 0.0f };
        /// Constructs a default line with both start + end set to zero
        CLine() = default;
        /// Constructs a line with the given start and end points
        CLine( const CVec3& p_start, const CVec3& p_end ) : start(p_start), end(p_end) {}
    };

    /// Returns the string representation of the given line object
    std::string ToString( const CLine& line );

    /// \brief Computes the remaining two axis of a reference frame, given the first axis and a world-up vector
    ///
    /// \param[in] axis1 First axis of the reference frame
    /// \param[out] axis2 Second (computed) axis of the reference frame
    /// \param[out] axis3 Third (computed) axis of the reference frame
    /// \param[in] worldUp World-up vector used for computing the reference frame
    void ComputeFrameAxes( const CVec3& axis1, CVec3& axis2, CVec3& axis3, const CVec3& worldUp );

    /// \brief Planer object, represented by a normal and a position vector
    struct CPlane
    {
        /// Normal of the plane
        CVec3 normal;
        /// A point on the plane
        CVec3 position;
    };

    /// \brief Returns the signed distance from the given point to the given plane
    ///
    /// \param[in] point Point from which we want to compute the distance
    /// \param[in] plane Plane to which we want to compute the distance from
    /// \return The signed distance from the point to the plane
    float32 SignedDistToPlane( const CVec3& point, const CPlane& plane );

    /// \brief Returns the distance from the given point to the given plane
    ///
    /// \param[in] point Point from which we want to compute the distance
    /// \param[in] plane Plane to which we watn to compute the distance from
    /// \return The distance from the point to the plane
    float32 DistToPlane( const CVec3& point, const CPlane& plane );

    /// \brief Returns the projection of a given point onto a given plane
    /// \param[in] point Point to be projected onto the plane
    /// \param[in] plane Plane on which the point is to be projected
    /// \return Vector representing the projection point
    CVec3 ProjInPlane( const CVec3& point, const CPlane& plane );

    /// \brief Axis-aligned bounding box object
    struct CBoundingBox
    {
        /// Size of the bounding box
        CVec3 size;
        /// World-transform of the reference frame attached to the center of the bounding box
        CMat4 tf;
    };

    /// \brief Returns the corners (8 vertices) of a given bounding box
    ///
    /// \param[in] bbox Bounding box from which to extract its corners
    /// \return An array containing the 8 corners of the bounding box
    std::array< CVec3, 8 > ComputeBoxCorners( const CBoundingBox& bbox );

    /// \brief Bounding sphere object
    struct CBoundingSphere
    {
        /// Radius of the bounding sphere
        float32 radius;
        /// Center in world-space of the bouding sphere
        CVec3 center;
    };

    /// \brief Frustum object, representing a view-frustum volume
    struct CFrustum
    {
        /// Corners of the view-frustum object
        std::array< CVec3, 8 > corners;
        /// Planes of the view-frustum object
        std::array< CPlane, 6 > planes;

        /// \brief Creates a frustum object from a given view-projection matrix
        ///
        /// \param[in] viewProjMat View-projection matrix used to construct the frustum object
        CFrustum( const CMat4& viewProjMat );
    };

    /// \brief Returns whether or not the given bounding box is "certainly" inside the frustum
    ///
    /// \param[in] frustum Frustum object with whom to check if the given box is certainly inside
    /// \param[in] bbox Bounding box object from which to check if it's certainly inside the frustum
    /// \return True if the bounding box is inside the frustum. False values don't necessarily mean outside of frustum
    bool CertainlyOutsideFrustum( const CFrustum& frustum, const CBoundingBox& bbox );

    /// \brief Returns whether or not the given bounding sphere is "certainly" inside the frustum
    ///
    /// \param[in] frustum Frustum object with whom to check if the given sphere is certainly inside
    /// \param[in] bbox Bounding sphere object from which to check if it's certainly inside the frustum
    /// \return True if the bounding sphere is inside the frustum. False values don't necessarily mean outside of frustum
    bool CertainlyOutsideFrustum( const CFrustum& frustum, const CBoundingSphere& bsphere );

    /// \brief Comparator class used for checking with the signed distance of points to a given plane
    struct CComparatorSignedDistancePlane
    {
        /// Plane from which to compare distances to
        CPlane plane;

        /// \brief Returns whether or not the first point's signed distance to the plane is the smallest
        bool operator() ( const CVec3& p1, const CVec3& p2 );
    };

    /// \brief Comparator class used for checking with the points x coordinate
    struct CComparatorX
    {
        /// \brief Returns whether or not the first point's x coordinate is the smallest
        bool operator() ( const CVec3& v1, const CVec3& v2 );
    };

    /// \brief Comparator class used for checking with the points y coordinate
    struct CComparatorY
    {
        /// \brief Returns whether or not the first point's y coordinate is the smallest
        bool operator() ( const CVec3& v1, const CVec3& v2 );
    };

    /// \brief Comparator class used for checking with the points z coordinate
    struct CComparatorZ
    {
        /// \brief Returns whether or not the first point's z coordinate is the smallest
        bool operator() ( const CVec3& v1, const CVec3& v2 );
    };

    /// \brief Comparator class used for checking which point is closest to a base given point
    struct CComparatorClosestToPoint
    {
        /// Point from which the distances are computed
        CVec3 point;

        /// \brief Returns whether or not the first point is closest to the base point
        bool operator() ( const CVec3& p1, const CVec3& p2 );
    };

    /// \brief Comparator class used for checking which point is farthest to a base given point
    struct CComparatorFarthestFromPoint
    {
        /// Point from which the distances are computed
        CVec3 point;

        /// \brief Returns whether or not the first point is farthest to the base point
        bool operator() ( const CVec3& p1, const CVec3& p2 );
    };

    /// \brief Gets the closest and farthest corner of a bounding box with respect to a plane
    void ComputeMinMaxVertexToPlane( const CPlane& plane, const CBoundingBox& bbox, CVec3& minVertex, CVec3& maxVertex );

    /// \brief Converts the given angle from degrees to radians
    float32 ToRadians( float32 angle );

    /// \brief Converts the given angle from radians to degrees
    float32 ToDegrees( float32 angle );
}
