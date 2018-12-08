

#include <LFpsCamera.h>

// TODO: Port the camera to a more intuitive implementation, e.g. speed.x should be speed.z

using namespace std;

namespace engine
{

    LFpsCamera::LFpsCamera( const string& name,
                            const LVec3& pos,
                            const LVec3& targetPoint,
                            int worldUpId,
                            float fov,
                            float aspectRatio,
                            float zNear, float zFar ) 
        : LICamera( name, pos, targetPoint, worldUpId, fov, aspectRatio, zNear, zFar )
    {
        m_roll = 0.0f;
        m_pitch = CAM_DEFAULT_PITCH;
        m_yaw = CAM_DEFAULT_YAW;

        m_rAngRot = 0.0f;
        m_uAngRot = 0.0f;

        m_baseSpeed = CAM_DEFAULT_SPEED;

        m_lastX = 0.0f;
        m_lastY = 0.0f;

        m_sensitivity = CAM_DEFAULT_SENSITIVITY;

        m_type = LFpsCamera::GetStaticType();

        _updateCameraVectors();
    }

    void LFpsCamera::update( float dt )
    {
        if ( !m_isActive )
        {
            return;
        }

        m_speed.x = m_speed.z = 0.0f;

        if ( InputSystem::isKeyDown( GLFW_KEY_W ) )
        {
            m_speed.x = m_baseSpeed;
        }
        else if ( InputSystem::isKeyDown( GLFW_KEY_S ) )
        {
            m_speed.x = -m_baseSpeed;
        }
        else if ( InputSystem::isKeyDown( GLFW_KEY_D ) )
        {
            m_speed.z = m_baseSpeed;
        }
        else if ( InputSystem::isKeyDown( GLFW_KEY_A ) )
        {
            m_speed.z = -m_baseSpeed;
        }


        // Update the camera angles
        LVec2 _pos = InputSystem::getCursorPosition();

        float _xOff = _pos.x - m_lastX;
        float _yOff = m_lastY - _pos.y;

        _xOff = ( _xOff > CAM_MAX_DELTA ? CAM_MAX_DELTA : ( _xOff < -CAM_MAX_DELTA ? -CAM_MAX_DELTA : _xOff ) );
        _yOff = ( _yOff > CAM_MAX_DELTA ? CAM_MAX_DELTA : ( _yOff < -CAM_MAX_DELTA ? -CAM_MAX_DELTA : _yOff ) );

        m_lastX = _pos.x;
        m_lastY = _pos.y;

        _xOff *= m_sensitivity;
        _yOff *= m_sensitivity;

        m_pitch += _yOff;
        m_yaw += _xOff;

        m_pitch = min( max( m_pitch, -89.0f ), 89.0f );

        _updateCameraVectors();

        LVec3 _dFront = m_front * ( m_speed.x * dt );
        LVec3 _dRight = m_right * ( m_speed.z * dt );

        m_pos = m_pos + _dFront + _dRight;
    }

    void LFpsCamera::_updateCameraVectors()
    {
        if ( m_worldUpVectorId == UP_X )
        {
            m_front.z = cos( toRadians( m_yaw ) ) * cos( toRadians( m_pitch ) );
            m_front.x = sin( toRadians( m_pitch ) );
            m_front.y = sin( toRadians( m_yaw ) ) * cos( toRadians( m_pitch ) );
        }
        else if ( m_worldUpVectorId == UP_Y )
        {
            m_front.x = cos( toRadians( m_yaw ) ) * cos( toRadians( m_pitch ) );
            m_front.y = sin( toRadians( m_pitch ) );
            m_front.z = sin( toRadians( m_yaw ) ) * cos( toRadians( m_pitch ) );
        }
        else if ( m_worldUpVectorId == UP_Z )
        {
            m_front.y = cos( toRadians( m_yaw ) ) * cos( toRadians( m_pitch ) );
            m_front.z = sin( toRadians( m_pitch ) );
            m_front.x = sin( toRadians( m_yaw ) ) * cos( toRadians( m_pitch ) );
        }

        m_front.normalize();

        m_right = LVec3::cross( m_front, m_worldUp );
        m_right.normalize();

        m_up = LVec3::cross( m_right, m_front );
        m_up.normalize();

        _buildViewMatrix();
    }

    void LFpsCamera::dumpInfo()
    {
        cout << "Camera base information for camera < " << m_name << " > *******" << endl;

        cout << "pos: " << m_pos.toString() << endl;
        cout << "targetDir: " << m_front.toString() << endl;
        cout << "worldUp: " << m_worldUp.toString() << endl;
        cout << "fov: " << m_fov << endl;
        cout << "aspectRatio: " << m_aspectRatio << endl;
        cout << "zNear: " << m_zNear << endl;
        cout << "zFar: " << m_zFar << endl;

        cout << "******************************************" << endl;
    }
}