#pragma once

#include <CCommon.h>
#include <CMath.h>

namespace engine
{
    /// Available camera types
    enum class eCameraType
    {
        /// Fixed camera type (look from fixed position to fixed target)
        FIXED = 0,
        /// First-person-shooting camera type (moves freely around 3d-space)
        FPS,
        /// Orbit camera type (rotates and moves in spherical coordinates around a fixed point)
        ORBIT
    };

    /// Returns the string representation of the given camera-type
    std::string ToString( const eCameraType& type );

    /// Available projection types
    enum class eCameraProjection
    {
        /// 3d-perspective projection type (pin-hole perspective like camera)
        PERSPECTIVE = 0,
        /// 3d-orthographic projection type (isometric-like camera)
        ORTHOGRAPHIC,
    };

    /// Returns the string representation of the given projection type
    std::string ToString( const eCameraProjection& proj );

    /// Structure describing projection parameters for a camera
    struct CCameraProjData
    {
        /// Type of projections used by the camera
        eCameraProjection projection = eCameraProjection::PERSPECTIVE;
        /// Field of view of the camera (for perspective cameras only)
        float fov = 45.0f;
        /// Aspect ratio of the inner frustum of the camera (for perspective cameras only)
        float aspect = 1024.0f / 768.0f;
        /// Width of view-box of the camera (for orthographic cameras only)
        float width = 1024.0f / 768.0f;
        /// Height of the view-box of the camera (for orthographics cameras only)
        float height = 1.0f;
        /// Distance of near-plane (closest) of view-volume of the camera
        float zNear = 0.1f;
        /// Distance of far-plane (furthest) of view-volume of the camera
        float zFar = 50.0f;
        /// Width of the viewport used for the frustum of the camera
        float viewportWidth = 1024.0f;
        /// Height of the viewport used for the frustum of the camera
        float viewportHeight = 768.0f;
    };

    /// Returns the string representation of a given projection-data struct
    std::string ToString( const CCameraProjData& proj_data );

    /// Camera interface for all 3d-camera types
    class CICamera
    {
    public :

        /// Base-constructor interface for all camera-types
        /// @param name         Unique-identifier of the camera in the scene
        /// @param position     Position of the reference-frame of the camera
        /// @param target_point Reference point that the camera looks at
        /// @param up_axis      World-up vector used to construct the camera's reference frame
        /// @param proj_data    Struct with projection-data parameters
        CICamera( const std::string& name,
                  const CVec3& position,
                  const CVec3& target_point,
                  const eAxis& up_axis,
                  const CCameraProjData& proj_data );

        /// Reeleases the resources allocated by this camera
        virtual ~CICamera();

        /// Changes the position of the reference-frame of the camera
        ///
        /// @param position     New position of the reference-frame of the camera
        void SetPosition( const CVec3& position );

        /// Changes the point the reference-frame of the camera points to
        ///
        /// @param target_point New target point the reference-frame will point to
        void SetTargetPoint( const CVec3& targetPoint );

        /// Either enables or disables the camera
        ///
        /// @param active   State to set the camera to (either enabled or disabled)
        void SetActiveMode( bool active );
        
        /// Updates the projection-data parameters
        ///
        /// @param proj_data    Struct containing projection-data parameters
        void SetProjectionData( const CCameraProjData& proj_data );

        /// Updates the internal-state of the camera
        void Update();

        /// Changes the aspect-ratio and projection parameters of the camera
        ///
        /// @param width    New witdh of the view-volume
        /// @param height   New height of the view-volume
        void Resize( int width, int height );

        /// Returns a string representation of the camera
        std::string ToString() const;

        /// Returns the unique-identifier of this camera in the scene
        std::string name() const { return m_Name; }

        /// Returns the enum-type of the camera
        eCameraType type() const { return m_Type; }

        /// Returns the enum-axis of the camera
        eAxis up_axis() const { return m_UpAxis; }

        /// Returns whether or not the camera is active
        bool active() const { return m_Active; }

        /// Returns the point the reference-frame of the camera points to (z-axis)
        CVec3 target_point() const { return m_TargetPoint; }

        /// Returns the z-axis of the reference frame of the camera (direction it points to)
        CVec3 target_direction() const { return m_TargetDir; }

        /// Returns the origin of the reference frame of the camera
        CVec3 position() const { return m_Position; }

        /// Returns a mutable reference to the projection parameters describing this camera
        CCameraProjData& proj_data() { return m_ProjData; }

        /// Returns an unmutable reference to the projection parameters describing this camera
        const CCameraProjData& proj_data() const { return m_ProjData; }

        /// Returns the view-matrix of this camera
        CMat4 mat_view() const { return m_MatView; }

        /// Returns the projection matrix of this camera
        CMat4 mat_proj() const { return m_MatProj; }

        /// Returns the z-axis of the reference frame of the camera
        CVec3 front() const { return m_Front; }

        /// Returns the y-axis of the reference frame of the camera
        CVec3 up() const { return m_Up; }

        /// Returns the x-axis of the reference frame of the camera
        CVec3 right() const { return m_Right; }

    protected :


        /// Updates the view-matrix for this camera
        void _BuildViewMatrix();

        /// Updates the projection matrix for this camera
        void _BuildProjMatrix();

        /// Called when this camera's position changes
        virtual void _PositionChangedInternal() = 0;

        /// Called when this camera's target changes
        virtual void _TargetPointChangedInternal() = 0;

        /// Called when this camera is updated
        virtual void _UpdateInternal() = 0;

        /// Called when this camera's viewport changes
        virtual void _ResizeInternal( int width, int height ) = 0;

        /// Returns the specific string representation of this camera
        virtual std::string _ToStringInternal() const = 0;

    protected :

        /// Unique identifier of the camera in the scene
        std::string m_Name;
        /// Type of this camera
        eCameraType m_Type;
        /// Axis used as world up axis
        eAxis m_UpAxis;
        /// Current position of this camera
        CVec3 m_Position;
        /// World up vector
        CVec3 m_WorldUp;
        /// Current position being pointed by this camera
        CVec3 m_TargetPoint;
        /// Current direction being pointed by this camera
        CVec3 m_TargetDir;
        /// Z-axis vector of the reference frame of this camera
        CVec3 m_Front;
        /// Y-axis vector of the reference frame of this camera
        CVec3 m_Up;
        /// X-axis vector of the reference frame of this camera
        CVec3 m_Right;
        /// View matrix of this camera
        CMat4 m_MatView;
        /// Projection matrix of this camera
        CMat4 m_MatProj;
        /// Projection parameters of this camera
        CCameraProjData m_ProjData;
        /// Flag that indicates whether or not this camera is currently active
        bool m_Active;
    };
}