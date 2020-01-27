#pragma once

#include <camera/CICamera.h>
#include <utils/CTime.h>
#include <input/CInputManager.h>

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
                      float viewportHeight,
                      float moveSensitivity = 0.005f,
                      float zoomSensitivity = 1.000f );

        static eCameraType GetStaticType() { return eCameraType::ORBIT; }

        void setMoveSensitivity( float moveSensitivity ) { m_moveSensitivity = moveSensitivity; }
        void setZoomSensitivity( float zoomSensitivity );

        float moveSensitivity() const { return m_moveSensitivity; }
        float zoomSensitivity() const { return m_zoomSensitivity; }

    protected :

        void _positionChangedInternal() override;
        void _targetPointChangedInternal() override;
        void _updateInternal() override;
        void _resizeInternal( int width, int height ) override;
        std::string _toStringInternal() const override;

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

        float m_baseAccumScroll;

        float m_moveSensitivity;
        float m_zoomSensitivity;

        eOrbitCameraState m_cameraState;
        CVec2 m_cursor;
        CVec2 m_cursor0;
    };

}