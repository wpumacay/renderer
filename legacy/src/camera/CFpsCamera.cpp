
#include <camera/CFpsCamera.h>

namespace engine
{
    CFpsCamera::CFpsCamera( const std::string& name,
                            const CVec3& position,
                            const CVec3& target_point,
                            const eAxis& up_axis,
                            const CCameraProjData& proj_data,
                            float sensitivity,
                            float speed,
                            float max_delta )
        : CICamera( name, position, target_point, up_axis, proj_data )
    {
        m_Type = eCameraType::FPS;

        m_Sensitivity = sensitivity;
        m_Speed = speed;
        m_MaxDelta = max_delta;

        m_Front = ( m_TargetPoint - m_Position ).normalized();
        m_Right = tinymath::cross( m_Front, m_WorldUp ).normalized();
        m_Up    = tinymath::cross( m_Right, m_Front ).normalized();
        _UpdateCameraAngles();
        _BuildViewMatrix();
    }

    void CFpsCamera::_UpdateInternal()
    {
        if ( !m_Active )
            return;

        m_CurrentFrontSpeed = 0.0f;
        m_CurrentRightSpeed = 0.0f;

        /**/ if ( CInputManager::IsKeyDown( engine::Keys::KEY_W ) ) m_CurrentFrontSpeed = m_Speed;
        else if ( CInputManager::IsKeyDown( engine::Keys::KEY_S ) ) m_CurrentFrontSpeed = -m_Speed;
        else if ( CInputManager::IsKeyDown( engine::Keys::KEY_D ) ) m_CurrentRightSpeed = m_Speed;
        else if ( CInputManager::IsKeyDown( engine::Keys::KEY_A ) ) m_CurrentRightSpeed = -m_Speed;

        /* compute camera angles from user cursor */
        CVec2 current_cursos_pos = CInputManager::GetCursorPosition();
        float x_off = current_cursos_pos.x() - m_LastCursorPos.x();
        float y_off = m_LastCursorPos.y() - current_cursos_pos.y();

        x_off = m_Sensitivity * std::min( m_MaxDelta, std::max( -m_MaxDelta, x_off ) );
        y_off = m_Sensitivity * std::min( m_MaxDelta, std::max( -m_MaxDelta, y_off ) );

        m_Yaw = m_Yaw + x_off;
        m_Pitch = std::min( std::max( m_Pitch + y_off, -89.0f ), 89.0f );

        // update camera frame of reference using these angles
        _UpdateCameraVectors();

        // and move a bit using this new reference frame
        auto dfront = m_Front * ( m_CurrentFrontSpeed * tinyutils::Clock::GetTimeStep() );
        auto dright = m_Right * ( m_CurrentRightSpeed * tinyutils::Clock::GetTimeStep() );
        m_Position = m_Position + dfront + dright;

        // some book keeping for next calculation
        m_LastCursorPos = current_cursos_pos;
    }

    std::string CFpsCamera::_ToStringInternal() const
    {
        std::string strrep;
        strrep += "front   : " + engine::toString( m_Front ) + "\n\r";
        strrep += "right   : " + engine::toString( m_Right ) + "\n\r";
        strrep += "up      : " + engine::toString( m_Up ) + "\n\r";
        strrep += "roll    : " + std::to_string( m_Roll ) + "\n\r";
        strrep += "pitch   : " + std::to_string( m_Pitch ) + "\n\r";
        strrep += "yaw     : " + std::to_string( m_Yaw ) + "\n\r";
        strrep += "senst.  : " + std::to_string( m_Sensitivity ) + "\n\r";
        strrep += "speed   : " + std::to_string( m_Speed ) + "\n\r";
        strrep += "speed-f : " + std::to_string( m_CurrentFrontSpeed ) + "\n\r";
        strrep += "speed-r : " + std::to_string( m_CurrentRightSpeed ) + "\n\r";
        return strrep;
    }

    void CFpsCamera::_UpdateCameraVectors()
    {
        if ( m_UpAxis == eAxis::X )
        {
            m_Front.x() = std::sin( toRadians( m_Pitch ) );
            m_Front.y() = std::cos( toRadians( m_Pitch ) ) * std::sin( toRadians( m_Yaw ) );
            m_Front.z() = std::cos( toRadians( m_Pitch ) ) * std::cos( toRadians( m_Yaw ) );
        }
        else if ( m_UpAxis == eAxis::Y )
        {
            m_Front.x() = std::cos( toRadians( m_Pitch ) ) * std::cos( toRadians( m_Yaw ) );
            m_Front.y() = std::sin( toRadians( m_Pitch ) );
            m_Front.z() = std::cos( toRadians( m_Pitch ) ) * std::sin( toRadians( m_Yaw ) );
        }
        else if ( m_UpAxis == eAxis::Z )
        {
            m_Front.x() = std::cos( toRadians( m_Pitch ) ) * std::sin( toRadians( m_Yaw ) );
            m_Front.y() = std::cos( toRadians( m_Pitch ) ) * std::cos( toRadians( m_Yaw ) );
            m_Front.z() = std::sin( toRadians( m_Pitch ) );
        }

        m_Front = m_Front.normalized();
        m_Right = tinymath::cross( m_Front, m_WorldUp ).normalized();
        m_Up    = tinymath::cross( m_Right, m_Front ).normalized();
        _BuildViewMatrix();
    }

    void CFpsCamera::_UpdateCameraAngles()
    {
        m_Roll  = 0.0f;
        m_Pitch = toDegrees( std::asin( m_Front.z() ) );
        m_Yaw   = toDegrees( std::atan2( m_Front.y(), m_Front.x() ) );
    }
}
