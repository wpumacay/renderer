#pragma once

#include <camera/CICamera.h>
#include <utils/CTime.h>
#include <input/CInputHandler.h>

namespace engine
{

    enum class eOrbitCameraState
    {
        IDLE = 0, 
        DRAGGING, 
        MOVING_TARGET
    };

    std::string toString( const eOrbitCameraState& state );

    class COrbitCamera : public CICamera
    {
    public :

        COrbitCamera( const std::string& name,
                      const CVec3& position,
                      const CVec3& targetPoint,
                      const eAxis& upAxis,
                      const CCameraProjData& projData,
                      float viewportWidth,
                      float viewportHeight );

        static eCameraType GetStaticType() { return eCameraType::ORBIT; }

    protected :

        void _positionChangedInternal() override;
        void _updateInternal() override;
        std::string _toStringInternal() override;

    private :

        void _computeSphericalsFromPosition();
        void _computePositionFromSphericals();
        void _updateCameraVectors();

    private :
        float m_viewportWidth;
        float m_viewportHeight;

        // spherical coordinates
        float m_rho;
        float m_theta;
        float m_phi;

        // reference sphericals used when dragging
        float m_rho0;
        float m_theta0;
        float m_phi0;

        CVec3 m_r;
        CVec3 m_targetPoint0;

        eOrbitCameraState m_cameraState;
        CVec2 m_cursor;
        CVec2 m_cursor0;
    };

}