
#include <camera/COrbitCamera.h>

namespace engine
{

    std::string toString( const eOrbitCameraState& state )
    {
        if ( state == eOrbitCameraState::IDLE ) return "idle";
        if ( state == eOrbitCameraState::DRAGGING ) return "dragging";
        if ( state == eOrbitCameraState::MOVING_TARGET ) return "moving_target";

        ENGINE_CORE_ASSERT( false, "Invalid orbit-camera-state given" );

        return "undefined";
    }

    COrbitCamera::COrbitCamera( const std::string& name,
                                const CVec3& position,
                                const CVec3& targetPoint,
                                const eAxis& upAxis,
                                const CCameraProjData& projData,
                                float viewportWidth,
                                float viewportHeight,
                                float moveSensitivity,
                                float zoomSensitivity )
        : CICamera( name, position, targetPoint, upAxis, projData )
    {
        m_type = eCameraType::ORBIT;
        m_viewportWidth = viewportWidth;
        m_viewportHeight = viewportHeight;

        m_rho = 0.0f;
        m_rho0 = 0.0f;
        m_theta = 0.0f;
        m_theta0 = 0.0f;
        m_phi = 0.0f;
        m_phi0 = 0.0f;

        m_r = { 0.0f, 0.0f, 0.0f };
        m_targetPoint0 = m_targetPoint;

        m_cameraState = eOrbitCameraState::IDLE;
        m_cursor = { 0.0f, 0.0f };
        m_cursor0 = { 0.0f, 0.0f };

        m_moveSensitivity = moveSensitivity;
        m_zoomSensitivity = zoomSensitivity;

        m_baseAccumScroll = m_zoomSensitivity * CInputManager::GetScrollAccumValueY();

        _computeSphericalsFromPosition();
        _updateCameraVectors();
        _buildViewMatrix();
    }

    void COrbitCamera::_positionChangedInternal()
    {
        _computeSphericalsFromPosition();
        _updateCameraVectors();
        _buildViewMatrix();
    }

    void COrbitCamera::_targetPointChangedInternal()
    {
        _computeSphericalsFromPosition();
        _updateCameraVectors();
        _buildViewMatrix();
    }

    void COrbitCamera::setZoomSensitivity( float zoomSensitivity )
    {
        m_zoomSensitivity = zoomSensitivity; 

        _computeSphericalsFromPosition();
        _updateCameraVectors();
        _buildViewMatrix();
    }

    void COrbitCamera::_updateInternal()
    {
        if ( !m_active )
            return;

        if ( m_cameraState == eOrbitCameraState::IDLE )
        {
            if ( CInputManager::IsMouseDown( Mouse::BUTTON_LEFT ) )
            {
                m_cameraState = eOrbitCameraState::DRAGGING;

                m_cursor0 = CInputManager::GetCursorPosition();
                m_cursor = CInputManager::GetCursorPosition();

                m_phi0 = m_phi;
                m_theta0 = m_theta;
            }
            else if ( CInputManager::IsMouseDown( Mouse::BUTTON_RIGHT ) )
            {
                m_cameraState = eOrbitCameraState::MOVING_TARGET;

                m_cursor0 = CInputManager::GetCursorPosition();
                m_cursor = CInputManager::GetCursorPosition();

                m_targetPoint0 = m_targetPoint;
            }
        }
        else if ( m_cameraState == eOrbitCameraState::DRAGGING )
        {
            m_cursor = CInputManager::GetCursorPosition();

            float _dx = m_cursor.x() - m_cursor0.x();
            float _dy = m_cursor.y() - m_cursor0.y();

            float _dtheta = ( -_dx / m_viewportWidth ) * 2.0f * ENGINE_PI;
            float _dphi = ( -_dy / m_viewportHeight ) * ENGINE_PI;

            m_theta = m_theta0 + _dtheta;
            m_phi = m_phi0 + _dphi;

            if ( !CInputManager::IsMouseDown( Mouse::BUTTON_LEFT ) )
                m_cameraState = eOrbitCameraState::IDLE;
        }
        else if ( m_cameraState == eOrbitCameraState::MOVING_TARGET )
        {
            m_cursor = CInputManager::GetCursorPosition();

            float _dx = -( m_cursor.x() - m_cursor0.x() );
            float _dy = m_cursor.y() - m_cursor0.y();

            m_targetPoint.x() = m_targetPoint0.x() + ( m_right.x() * _dx + m_up.x() * _dy ) * m_moveSensitivity;
            m_targetPoint.y() = m_targetPoint0.y() + ( m_right.y() * _dx + m_up.y() * _dy ) * m_moveSensitivity;
            m_targetPoint.z() = m_targetPoint0.z();

            if ( !CInputManager::IsMouseDown( Mouse::BUTTON_RIGHT ) )
                m_cameraState = eOrbitCameraState::IDLE;
        }

        m_rho = m_rho0 + ( m_baseAccumScroll - m_zoomSensitivity * CInputManager::GetScrollAccumValueY() ) * 0.25f;

        _computePositionFromSphericals();
        _updateCameraVectors();
        _buildViewMatrix();
    }

    void COrbitCamera::_resizeInternal( int width, int height )
    {
        m_viewportWidth = width;
        m_viewportHeight = height;
    }

    std::string COrbitCamera::_toStringInternal() const
    {
        std::string _strRep;

        _strRep += "state   : " + engine::toString( m_cameraState ) + "\n\r";
        _strRep += "front   : " + engine::toString( m_front ) + "\n\r";
        _strRep += "right   : " + engine::toString( m_right ) + "\n\r";
        _strRep += "up      : " + engine::toString( m_up ) + "\n\r";
        _strRep += "rho     : " + std::to_string( m_rho ) + "\n\r";
        _strRep += "rho0    : " + std::to_string( m_rho0 ) + "\n\r";
        _strRep += "phi     : " + std::to_string( m_phi ) + "\n\r";
        _strRep += "phi0    : " + std::to_string( m_phi0 ) + "\n\r";
        _strRep += "theta   : " + std::to_string( m_theta ) + "\n\r";
        _strRep += "theta0  : " + std::to_string( m_theta0 ) + "\n\r";
        _strRep += "r       : " + engine::toString( m_r ) + "\n\r";
        _strRep += "vpwidth : " + std::to_string( m_viewportWidth ) + "\n\r";
        _strRep += "vpheight: " + std::to_string( m_viewportHeight ) + "\n\r";

        return _strRep;
    }

    void COrbitCamera::_computeSphericalsFromPosition()
    {
        m_r = m_position - m_targetPoint;

        m_rho0 = m_rho = m_r.length();
        if ( m_upAxis == eAxis::X )
        {
            m_phi0 = m_phi = std::acos( m_r.x() / m_rho0 );
            m_theta0 = m_theta = std::atan2( m_r.z(), m_r.y() );
        }
        else if ( m_upAxis == eAxis::Y )
        {
            m_phi0 = m_phi = std::acos( m_r.y() / m_rho0 );
            m_theta0 = m_theta = std::atan2( m_r.x(), m_r.z() );
        }
        else if ( m_upAxis == eAxis::Z )
        {
            m_phi0 = m_phi = std::acos( m_r.z() / m_rho0 );
            m_theta0 = m_theta = std::atan2( m_r.y(), m_r.x() );
        }

        m_baseAccumScroll = m_zoomSensitivity * CInputManager::GetScrollAccumValueY();
    }

    void COrbitCamera::_computePositionFromSphericals()
    {
        float _sphi = std::sin( m_phi );
        float _cphi = std::cos( m_phi );
        float _stheta = std::sin( m_theta );
        float _ctheta = std::cos( m_theta );

        if ( m_upAxis == eAxis::X )
        {
            m_r.x() = m_rho * _cphi;
            m_r.y() = m_rho * _sphi * _ctheta;
            m_r.z() = m_rho * _sphi * _stheta;
        }
        else if ( m_upAxis == eAxis::Y )
        {
            m_r.x() = m_rho * _sphi * _stheta;
            m_r.y() = m_rho * _cphi;
            m_r.z() = m_rho * _sphi * _ctheta;
        }
        else if ( m_upAxis == eAxis::Z )
        {
            m_r.x() = m_rho * _sphi * _ctheta;
            m_r.y() = m_rho * _sphi * _stheta;
            m_r.z() = m_rho * _cphi;
        }

        m_position = m_targetPoint + m_r;
    }

    void COrbitCamera::_updateCameraVectors()
    {
        m_front = ( m_targetPoint - m_position ).normalized();
        m_right = tinymath::cross( m_front, m_worldUp ).normalized();
        m_up    = tinymath::cross( m_right, m_front ).normalized();
    }

}