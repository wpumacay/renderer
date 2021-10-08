
#include <camera/COrbitCamera.h>

namespace engine
{
    std::string ToString( const eOrbitCameraState& state )
    {
        /**/ if ( state == eOrbitCameraState::IDLE ) return "idle";
        else if ( state == eOrbitCameraState::DRAGGING ) return "dragging";
        else if ( state == eOrbitCameraState::MOVING_TARGET ) return "moving_target";

        ENGINE_CORE_CRITICAL( "Invalid orbit-camera state" );
        return "undefined";
    }

    COrbitCamera::COrbitCamera( const std::string& name,
                                const CVec3& position,
                                const CVec3& target_point,
                                const eAxis& up_axis,
                                const CCameraProjData& proj_data,
                                float move_sensitivity,
                                float zoom_sensitivity )
        : CICamera( name, position, target_point, up_axis, proj_data )
    {
        m_Type = eCameraType::ORBIT;

        m_TargetPoint0 = m_TargetPoint;

        m_MoveSensitivity = move_sensitivity;
        m_ZoomSensitivity = zoom_sensitivity;

        _ComputeSphericalsFromPosition();
        _UpdateCameraVectors();
        _BuildViewMatrix();
    }

    void COrbitCamera::_PositionChangedInternal()
    {
        _ComputeSphericalsFromPosition();
        _UpdateCameraVectors();
        _BuildViewMatrix();
    }

    void COrbitCamera::_TargetPointChangedInternal()
    {
        _ComputeSphericalsFromPosition();
        _UpdateCameraVectors();
        _BuildViewMatrix();
    }

    void COrbitCamera::_UpdateInternal()
    {
        if ( !m_Active )
            return;

        if ( m_State == eOrbitCameraState::IDLE )
        {
            if ( CInputManager::IsMouseDown( Mouse::BUTTON_LEFT ) )
            {
                m_State = eOrbitCameraState::DRAGGING;

                m_Cursor0 = CInputManager::GetCursorPosition();
                m_Cursor = CInputManager::GetCursorPosition();

                m_Phi0 = m_Phi;
                m_Theta0 = m_Theta;
            }
            else if ( CInputManager::IsMouseDown( Mouse::BUTTON_RIGHT ) )
            {
                m_State = eOrbitCameraState::MOVING_TARGET;

                m_Cursor0 = CInputManager::GetCursorPosition();
                m_Cursor = CInputManager::GetCursorPosition();

                m_TargetPoint0 = m_TargetPoint;
            }
        }
        else if ( m_State == eOrbitCameraState::DRAGGING )
        {
            m_Cursor = CInputManager::GetCursorPosition();

            float _dx = m_Cursor.x() - m_Cursor0.x();
            float _dy = m_Cursor.y() - m_Cursor0.y();

            float _dtheta = ( -_dx / m_ProjData.viewportWidth ) * 2.0f * engine::PI;
            float _dphi = ( -_dy / m_ProjData.viewportHeight ) * engine::PI;

            m_Theta = m_Theta0 + _dtheta;
            m_Phi = m_Phi0 + _dphi;

            if ( !CInputManager::IsMouseDown( Mouse::BUTTON_LEFT ) )
                m_State = eOrbitCameraState::IDLE;
        }
        else if ( m_State == eOrbitCameraState::MOVING_TARGET )
        {
            m_Cursor = CInputManager::GetCursorPosition();

            float _dx = -( m_Cursor.x() - m_Cursor0.x() );
            float _dy = m_Cursor.y() - m_Cursor0.y();

            m_TargetPoint.x() = m_TargetPoint0.x() + ( m_Right.x() * _dx + m_Up.x() * _dy ) * m_MoveSensitivity;
            m_TargetPoint.y() = m_TargetPoint0.y() + ( m_Right.y() * _dx + m_Up.y() * _dy ) * m_MoveSensitivity;
            m_TargetPoint.z() = m_TargetPoint0.z();

            if ( !CInputManager::IsMouseDown( Mouse::BUTTON_RIGHT ) )
                m_State = eOrbitCameraState::IDLE;
        }

        m_Rho = m_Rho0 + ( m_ZoomSensitivity * CInputManager::GetScrollAccumValueY() ) * 0.25f;

        _ComputePositionFromSphericals();
        _UpdateCameraVectors();
        _BuildViewMatrix();
    }

    std::string COrbitCamera::_ToStringInternal() const
    {
        std::string strrep;
        strrep += "state   : " + engine::ToString( m_State ) + "\n\r";
        strrep += "front   : " + engine::toString( m_Front ) + "\n\r";
        strrep += "right   : " + engine::toString( m_Right ) + "\n\r";
        strrep += "up      : " + engine::toString( m_Up ) + "\n\r";
        strrep += "rho     : " + std::to_string( m_Rho ) + "\n\r";
        strrep += "rho0    : " + std::to_string( m_Rho0 ) + "\n\r";
        strrep += "phi     : " + std::to_string( m_Phi ) + "\n\r";
        strrep += "phi0    : " + std::to_string( m_Phi ) + "\n\r";
        strrep += "theta   : " + std::to_string( m_Theta ) + "\n\r";
        strrep += "theta0  : " + std::to_string( m_Theta0 ) + "\n\r";
        strrep += "r       : " + engine::toString( m_Radial ) + "\n\r";
        return strrep;
    }

    void COrbitCamera::_ComputeSphericalsFromPosition()
    {
        m_Radial = m_Position - m_TargetPoint;
        m_Rho0 = m_Rho = m_Radial.length();
        if ( m_UpAxis == eAxis::X )
        {
            m_Phi0 = m_Phi = std::acos( m_Radial.x() / m_Rho0 );
            m_Theta0 = m_Theta = std::atan2( m_Radial.z(), m_Radial.y() );
        }
        else if ( m_UpAxis == eAxis::Y )
        {
            m_Phi0 = m_Phi = std::acos( m_Radial.y() / m_Rho0 );
            m_Theta0 = m_Theta = std::atan2( m_Radial.x(), m_Radial.z() );
        }
        else if ( m_UpAxis == eAxis::Z )
        {
            m_Phi0 = m_Phi = std::acos( m_Radial.z() / m_Rho0 );
            m_Theta0 = m_Theta = std::atan2( m_Radial.y(), m_Radial.x() );
        }
    }

    void COrbitCamera::_ComputePositionFromSphericals()
    {
        const float _sphi = std::sin( m_Phi );
        const float _cphi = std::cos( m_Phi );
        const float _stheta = std::sin( m_Theta );
        const float _ctheta = std::cos( m_Theta );

        if ( m_UpAxis == eAxis::X )
        {
            m_Radial.x() = m_Rho * _cphi;
            m_Radial.y() = m_Rho * _sphi * _ctheta;
            m_Radial.z() = m_Rho * _sphi * _stheta;
        }
        else if ( m_UpAxis == eAxis::Y )
        {
            m_Radial.x() = m_Rho * _sphi * _stheta;
            m_Radial.y() = m_Rho * _cphi;
            m_Radial.z() = m_Rho * _sphi * _ctheta;
        }
        else if ( m_UpAxis == eAxis::Z )
        {
            m_Radial.x() = m_Rho * _sphi * _ctheta;
            m_Radial.y() = m_Rho * _sphi * _stheta;
            m_Radial.z() = m_Rho * _cphi;
        }
        m_Position = m_TargetPoint + m_Radial;
    }

    void COrbitCamera::_UpdateCameraVectors()
    {
        m_Front = ( m_TargetPoint - m_Position ).normalized();
        m_Right = tinymath::cross( m_Front, m_WorldUp ).normalized();
        m_Up    = tinymath::cross( m_Right, m_Front ).normalized();
    }
}
