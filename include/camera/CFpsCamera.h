#pragma once

#include <camera/CICamera.h>
#include <utils/CTime.h>
#include <input/CInputHandler.h>

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
                    float sensitivity,
                    float camSpeed,
                    float camMaxDelta );

        static eCameraType GetStaticType() { return eCameraType::FPS; }

        void setSensitivity( float sensitivity ) { m_sensitivity = sensitivity; }
        void setCamSpeed( float camSpeed ) { m_camSpeed = camSpeed; }
        void setCamMaxDelta( float camMaxDelta ) { m_camMaxDelta = camMaxDelta; }

        float sensitivity() const { return m_sensitivity; }
        float camSpeed() const { return m_camSpeed; }
        float camMaxDelta() const { return m_camMaxDelta; }

        float camSpeedFront() const { return m_camSpeedFront; }
        float camSpeedRight() const { return m_camSpeedRight; }
        float roll() const { return m_roll; }
        float pitch() const { return m_pitch; }
        float yaw() const { return m_yaw; }


    protected :

        void _positionChangedInternal() override;
        void _updateInternal() override;
        std::string _toStringInternal() override;

    private :

        void _updateCameraVectors();
        void _updateCameraAngles();

    private :

        float m_roll;
        float m_pitch;
        float m_yaw;

        float m_sensitivity;
        float m_camSpeedFront;
        float m_camSpeedRight;
        float m_camSpeed;
        float m_camMaxDelta;

        CVec2 m_lastCursorPos;
    };
}