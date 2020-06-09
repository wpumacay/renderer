#pragma once

#include <camera/CICamera.h>
#include <input/CInputManager.h>

namespace engine
{

    class CFpsCamera : public CICamera
    {
    public :

        CFpsCamera( const std::string& name,
                    const CVec3& position,
                    const CVec3& targetPoint,
                    const eAxis& upAxis,
                    const CCameraProjData& projData,
                    float sensitivity = 0.25f,
                    float speed = 250.0f,
                    float max_delta = 10.0f );

        void SetSensitivity( float sensitivity ) { m_Sensitivity = sensitivity; }

        void SetSpeed( float speed ) { m_Speed = speed; }

        void SetMaxDelta( float max_delta ) { m_MaxDelta = max_delta; }

        float sensitivity() const { return m_Sensitivity; }

        float speed() const { return m_Speed; }

        float max_delta() const { return m_MaxDelta; }

        float current_front_speed() const { return m_CurrentFrontSpeed; }

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

        float m_Roll = 0.0f;

        float m_Pitch = 0.0f;

        float m_Yaw = -90.0f;

        float m_Sensitivity = 0.25f;

        float m_MaxDelta = 10.0f;

        float m_Speed = 250.0f;

        float m_CurrentFrontSpeed = 0.0f;

        float m_CurrentRightSpeed = 0.0f;

        CVec2 m_LastCursorPos = { 0.0f, 0.0f };
    };
}