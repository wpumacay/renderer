
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
                                const LVec3& position,
                                const LVec3& targetPoint,
                                const eAxis& upAxis,
                                const CCameraProjData& projData,
                                float viewportWidth,
                                float viewportHeight )
        : CICamera( name, position, targetPoint, upAxis, projData )
    {
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

    void COrbitCamera::_updateInternal()
    {
        if ( !m_active )
            return;

        if ( m_cameraState == eOrbitCameraState::IDLE )
        {
            if ( InputSystem::isMouseDown( ENGINE_MOUSE_BUTTON_LEFT ) )
            {
                m_cameraState = eOrbitCameraState::DRAGGING;

                m_cursor0 = InputSystem::getCursorPosition();
                m_cursor = InputSystem::getCursorPosition();

                m_phi0 = m_phi;
                m_theta0 = m_theta;
            }
            else if ( InputSystem::isMouseDown( ENGINE_MOUSE_BUTTON_RIGHT ) )
            {
                m_cameraState = eOrbitCameraState::MOVING_TARGET;

                m_cursor0 = InputSystem::getCursorPosition();
                m_cursor = InputSystem::getCursorPosition();

                m_targetPoint0 = m_targetPoint;
            }
        }
        else if ( m_cameraState == eOrbitCameraState::DRAGGING )
        {
            m_cursor = InputSystem::getCursorPosition();

            float _dx = m_cursor.x - m_cursor0.x;
            float _dy = m_cursor.y - m_cursor0.y;

            float _dtheta = ( -_dx / m_viewportWidth ) * 2.0f * _PI;
            float _dphi = ( -_dy / m_viewportHeight ) * _PI;

            m_theta = m_theta0 + _dtheta;
            m_phi = m_phi0 + _dphi;

            if ( !InputSystem::isMouseDown( ENGINE_MOUSE_BUTTON_LEFT ) )
                m_cameraState = eOrbitCameraState::IDLE;
        }
        else if ( m_cameraState == eOrbitCameraState::MOVING_TARGET )
        {
            m_cursor = InputSystem::getCursorPosition();

            float _dx = -( m_cursor.x - m_cursor0.x );
            float _dy = m_cursor.y - m_cursor0.y;

            m_targetPoint.x = m_targetPoint0.x + ( m_right.x * _dx + m_up.x * _dy ) * 0.005f;
            m_targetPoint.y = m_targetPoint0.y + ( m_right.y * _dx + m_up.y * _dy ) * 0.005f;
            m_targetPoint.z = m_targetPoint0.z;

            if ( !InputSystem::isMouseDown( ENGINE_MOUSE_BUTTON_RIGHT ) )
                m_cameraState = eOrbitCameraState::IDLE;
        }

        m_rho = m_rho0 - InputSystem::getScrollAccumValueY() * 0.25f;

        _computePositionFromSphericals();
        _updateCameraVectors();
        _buildViewMatrix();
    }

    std::string COrbitCamera::_toStringInternal()
    {
        std::string _strRep;

        _strRep += "state   : " + engine::toString( m_cameraState ) + "\n\r";
        _strRep += "front   : " + m_front.toString() + "\n\r";
        _strRep += "right   : " + m_right.toString() + "\n\r";
        _strRep += "up      : " + m_up.toString() + "\n\r";
        _strRep += "rho     : " + std::to_string( m_rho ) + "\n\r";
        _strRep += "rho0    : " + std::to_string( m_rho0 ) + "\n\r";
        _strRep += "phi     : " + std::to_string( m_phi ) + "\n\r";
        _strRep += "phi0    : " + std::to_string( m_phi0 ) + "\n\r";
        _strRep += "theta   : " + std::to_string( m_theta ) + "\n\r";
        _strRep += "theta0  : " + std::to_string( m_theta0 ) + "\n\r";
        _strRep += "r       : " + m_r.toString() + "\n\r";
        _strRep += "vpwidth : " + std::to_string( m_viewportWidth ) + "\n\r";
        _strRep += "vpheight: " + std::to_string( m_viewportHeight ) + "\n\r";

        return _strRep;
    }

    void COrbitCamera::_computeSphericalsFromPosition()
    {
        m_r = m_position - m_targetPoint;

        m_rho0 = m_rho = m_r.length();
        m_phi0 = m_phi = std::acos( m_r.z / m_rho );
        m_theta0 = m_theta = std::atan2( m_r.y, m_r.x );
    }

    void COrbitCamera::_computePositionFromSphericals()
    {
        float _sphi = std::sin( m_phi );
        float _cphi = std::cos( m_phi );
        float _stheta = std::sin( m_theta );
        float _ctheta = std::cos( m_theta );

        if ( m_upAxis == eAxis::X )
        {
            m_r.x = m_rho * _cphi;
            m_r.y = m_rho * _sphi * _ctheta;
            m_r.z = m_rho * _sphi * _stheta;
        }
        else if ( m_upAxis == eAxis::Y )
        {
            m_r.x = m_rho * _sphi * _stheta;
            m_r.y = m_rho * _cphi;
            m_r.z = m_rho * _sphi * _ctheta;
        }
        else if ( m_upAxis == eAxis::Z )
        {
            m_r.x = m_rho * _sphi * _ctheta;
            m_r.y = m_rho * _sphi * _stheta;
            m_r.z = m_rho * _cphi;
        }

        m_position = m_targetPoint + m_r;
    }

    void COrbitCamera::_updateCameraVectors()
    {
        m_front = LVec3::normalize( m_targetPoint - m_position );
        m_right = LVec3::normalize( LVec3::cross( m_front, m_worldUp ) );
        m_up    = LVec3::normalize( LVec3::cross( m_right, m_front ) );
    }

}