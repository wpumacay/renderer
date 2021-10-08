#pragma once

#include <camera/CICamera.h>
#include <input/CInputManager.h>

namespace engine
{
    /// First Person Shooter type of camera, which the user can interact with by using the mouse and keyboard.
    /// This type of camera is similar to the ones used in FPS games, in which the user controls the free
    /// movement of the reference frame of the camera in 3D space using the W,S,A,D keys for forward, backward,
    /// left, and right respectively. Users can also control the orientation of the reference frame of the
    /// camera by using the mouse cursor to point to a specific direction
    class CFpsCamera : public CICamera
    {
    public :

        /// Creates a first-person-shooter camera, at a starting position and pointing to a given target
        ///
        /// @param name             Unique identifier of the camera in the scene
        /// @param position         Position in world-space of this camera
        /// @param target_point     Point in world-space that this camera points to
        /// @param up_axis          Axis used as world-up in the scene
        /// @param proj_data        Structs containing the parameters describing projection information
        /// @param sensitivity      Sensitivity of the mouse to the movement of the mouse cursor
        /// @param speed            Base speed for the movement of the camera in 3D space
        /// @param max_delta        Maximum value difference of the movement of the cursor in between timesteps
        CFpsCamera( const std::string& name,
                    const CVec3& position,
                    const CVec3& target_point,
                    const eAxis& up_axis,
                    const CCameraProjData& proj_data,
                    float sensitivity = 0.25f,
                    float speed = 250.0f,
                    float max_delta = 10.0f );

        /// Sets the mouse sensitivity to the user movement of the cursor
        void SetSensitivity( float sensitivity ) { m_Sensitivity = sensitivity; }

        /// Sets the base speed value for the movement of the camera
        void SetSpeed( float speed ) { m_Speed = speed; }

        /// Sets the maximum value of the movement of the cursor in between timesteps
        void SetMaxDelta( float max_delta ) { m_MaxDelta = max_delta; }

        /// Returns the sensitivity of the mouse to the user movement of the cursor
        float sensitivity() const { return m_Sensitivity; }

        /// Returns the base speed of the movement of the camera
        float speed() const { return m_Speed; }

        /// Returns the maximum value difference of the movement of the cursor in between timesteps
        float max_delta() const { return m_MaxDelta; }

        /// Returns the current value of the speed in the front direction (camera's Z-axis)
        float current_front_speed() const { return m_CurrentFrontSpeed; }

        /// Returns the current value of the speed in the right direction (camera's X-axis)
        float current_right_speed() const { return m_CurrentRightSpeed; }

        /// Returns the roll angle of the reference frame of the camera
        float roll() const { return m_Roll; }

        /// Returns the pitch angle of the reference frame of the camera
        float pitch() const { return m_Pitch; }

        /// Returns the yaw angle of the reference frame of the camera
        float yaw() const { return m_Yaw; }

        // Documentation inherited
        static eCameraType GetStaticType() { return eCameraType::FPS; }

    protected :

        // Documentation inherited
        void _PositionChangedInternal() override { /* Do nothing, keep same frame vectors */ }

        // Documentation inherited
        void _TargetPointChangedInternal() override { /* Do nothing, keep same frame vectors */ }

        // Documentation inherited
        void _UpdateInternal() override;

        // Documentation inherited
        void _ResizeInternal( int width, int height ) override {}

        // Documentation inherited
        std::string _ToStringInternal() const override;

    private :

        /// Updates the unit vectors that represent the reference frame of the camera
        void _UpdateCameraVectors();

        /// Updates the angles (roll,pitch,yaw) that define the orientation of the reference frame of the camera
        void _UpdateCameraAngles();

    private :

        /// Roll angle for the orientation of the camera's reference frame
        float m_Roll = 0.0f;
        /// Pitch angle for the orientation of the camera's reference frame
        float m_Pitch = 0.0f;
        /// Yaw angle for the orientation of the camera's reference frame
        float m_Yaw = -90.0f;
        /// Mouse sensitivity to the movement of the camera using the mouse cursor
        float m_Sensitivity = 0.25f;
        /// Maximum value for clamping the movement of the mouse cursor in between timesteps
        float m_MaxDelta = 10.0f;
        /// Base speed value for the movement of the camera
        float m_Speed = 250.0f;
        /// Current value of the speed of the camera along the front direction
        float m_CurrentFrontSpeed = 0.0f;
        /// Current value of the speec of the camera along the right direction
        float m_CurrentRightSpeed = 0.0f;
        /// Last recorded position of the mouse cursor
        CVec2 m_LastCursorPos = { 0.0f, 0.0f };
    };
}
