#pragma once

#include <camera/CICamera.h>
#include <input/CInputManager.h>

namespace engine
{
    /// All valid states in which orbit cameras can be in
    enum class eOrbitCameraState
    {
        /// Idle state, when the user is not interacting with the camera
        IDLE = 0,
        /// Dragging state, when the user is interacting by left-click dragging with the mouse
        DRAGGING,
        /// Moving-target state, when the user is interacting by right-click draging with the mouse
        MOVING_TARGET
    };

    /// Returns the string representation of the given orbit-camera state
    std::string ToString( const eOrbitCameraState& state );

    /// Orbit-type of camera, which the user can interact with by using the mouse. This type of camera
    /// is very similar to the ones used in CAD tools, where the user can alter the view in spherical
    /// coordinates around a point (orbit around a point) using the left-mouse button, and pan as well
    /// by dragging with the right-mouse button
    class COrbitCamera : public CICamera
    {
    public :

        /// Creates an orbit camera, at a starting position and pointing to a given target
        ///
        /// @param name             Unique identifier of the camera in the scene
        /// @param position         Position in world-space of this camera
        /// @param target_point     Point in world-space that this camera points to
        /// @param up_axis          Axis used as world-up in the scene
        /// @param proj_data        Structs containing the parameters describing projection information
        /// @param move_sensitivity Sensitivity to the movement by dragging with the left mouse button
        /// @param zoom_sensitivity Sentitivity of the movement by scrolling with the mouse wheel
        COrbitCamera( const std::string& name,
                      const CVec3& position,
                      const CVec3& target_point,
                      const eAxis& up_axis,
                      const CCameraProjData& proj_data,
                      float move_sensitivity = 0.005f,
                      float zoom_sensitivity = 1.000f );

        /// Sets the sensitivity of the movement of the left mouse button dragging
        void SetMoveSensitivity( float move_sensitivity ) { m_MoveSensitivity = move_sensitivity; }

        /// Sets the sensitivity of the movement of the scrolling of the mouse wheel
        void SetZoomSensitivity( float zoom_sensitivity ) { m_ZoomSensitivity = zoom_sensitivity; }

        /// Returns the sensitivity of the movement of the left mouse button dragging
        float move_sensitivity() const { return m_MoveSensitivity; }

        /// Returns the senssitivity of the movement of the scrolling of the mouse wheel
        float zoom_sensitivity() const { return m_ZoomSensitivity; }

        // Documentation inherited
        static eCameraType GetStaticType() { return eCameraType::ORBIT; }

    protected :

        // Documentation inherited
        void _PositionChangedInternal() override;

        // Documentation inherited
        void _TargetPointChangedInternal() override;

        // Documentation inherited
        void _UpdateInternal() override;

        // Documentation inherited
        void _ResizeInternal( int width, int height ) override {}

        // Documentation inherited
        std::string _ToStringInternal() const override;

    private :

        /// Updates the internal spherical coordinate representation once the position of the camera changes
        void _ComputeSphericalsFromPosition();

        /// Updates the internal position once the spherical coordinates of the camera change
        void _ComputePositionFromSphericals();

        /// Updates the axis-vectors of the reference frame of this camera its internal state
        void _UpdateCameraVectors();

    private :
        /// Radial distance (Rho spherical coordinate)
        float m_Rho = 0.0f;
        /// Radial angle (Theta spherical coordinate)
        float m_Theta = 0.0f;
        /// Elevation angle (Phi spherical coordinate)
        float m_Phi = 0.0f;
        /// Initial value of the radial distance
        float m_Rho0 = 0.0f;
        /// Initial value of the radial angle
        float m_Theta0 = 0.0f;
        /// Initial value of the elevation angle
        float m_Phi0 = 0.0f;
        /// Vector from camera's origin to the target position
        CVec3 m_Radial = { 0.0f, 0.0f };
        /// Initial value of the target position
        CVec3 m_TargetPoint0 = { 0.0f, 0.0f, 0.0f };
        /// Current value of the cursor(mouse) position in screen coordinates
        CVec2 m_Cursor = { 0.0f, 0.0f };
        /// Initial value of the cursor(mouse) position in screen coordinates
        CVec2 m_Cursor0 = { 0.0f, 0.0f };
        /// Value of the sensitivity of the camera to the movement using the left mouse button (dragging)
        float m_MoveSensitivity = 0.005f;
        /// Value of the sensitivity of the camera to the scrolling using the mouse wheel
        float m_ZoomSensitivity = 1.000f;
        /// Current state of the state machine controller of the camera
        eOrbitCameraState m_State = eOrbitCameraState::IDLE;
    };

}