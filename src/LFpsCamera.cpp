

#include "LFpsCamera.h"

// TODO: Port the camera to a more intuitive implementation, e.g. speed.x should be speed.z

using namespace std;

namespace engine
{

    LFpsCamera::LFpsCamera( const LVec3& pos,
                            const LVec3& worldUp,
                            float fov,
                            float aspectRatio,
                            float zNear, float zFar ) 
        : LICamera( pos, LVec3(), worldUp, fov, aspectRatio, zNear, zFar )
    {
        m_roll = 0.0f;
        m_pitch = CAM_DEFAULT_PITCH;
        m_yaw = CAM_DEFAULT_YAW;

        m_baseSpeed = CAM_DEFAULT_SPEED;

        m_lastX = 0.0f;
        m_lastY = 0.0f;

        m_sensitivity = CAM_DEFAULT_SENSITIVITY;

        m_type = LFpsCamera::getStaticType();

        _updateCamera();
    }

    glm::mat4 LFpsCamera::getViewMatrix()
    {
        return glm::lookAt( glm::vec3( m_pos.x, m_pos.y, m_pos.z ),
                            glm::vec3( m_pos.x + m_front.x, 
                                       m_pos.y + m_front.y,
                                       m_pos.z + m_front.z ),
                            glm::vec3( m_up.x, m_up.y, m_up.z ) );
    }

    void LFpsCamera::update( float dt )
    {
        m_speed.x = m_speed.z = 0.0f;

        if ( LInputHandler::INSTANCE->isKeyPressed( GLFW_KEY_W ) )
        {
            m_speed.x = m_baseSpeed;
        }
        else if ( LInputHandler::INSTANCE->isKeyPressed( GLFW_KEY_S ) )
        {
            m_speed.x = -m_baseSpeed;
        }
        else if ( LInputHandler::INSTANCE->isKeyPressed( GLFW_KEY_D ) )
        {
            m_speed.z = m_baseSpeed;
        }
        else if ( LInputHandler::INSTANCE->isKeyPressed( GLFW_KEY_A ) )
        {
            m_speed.z = -m_baseSpeed;
        }


        // Update the camera angles
        float x, y;
        LInputHandler::INSTANCE->getMousePosition( x, y );

        float _xOff = x - m_lastX;
        float _yOff = m_lastY - y;

        _xOff = ( _xOff > CAM_MAX_DELTA ? CAM_MAX_DELTA : ( _xOff < -CAM_MAX_DELTA ? -CAM_MAX_DELTA : _xOff ) );
        _yOff = ( _yOff > CAM_MAX_DELTA ? CAM_MAX_DELTA : ( _yOff < -CAM_MAX_DELTA ? -CAM_MAX_DELTA : _yOff ) );

        m_lastX = x;
        m_lastY = y;

        _xOff *= m_sensitivity;
        _yOff *= m_sensitivity;

        m_pitch += _yOff;
        m_yaw += _xOff;

        m_pitch = min( max( m_pitch, -89.0f ), 89.0f );

        _updateCamera();

        LVec3 _dFront = m_front * ( m_speed.x * dt );
        LVec3 _dRight = m_right * ( m_speed.z * dt );

        m_pos = m_pos + _dFront + _dRight;


        //std::cout << "pitch: " << m_pitch << std::endl;
        //std::cout << "yaw: " << m_yaw << std::endl;

        // std::cout << "cy: " << m_pos.y << std::endl;
        // std::cout << "cx: " << m_pos.x << std::endl;
        // std::cout << "cz: " << m_pos.z << std::endl;
    }

    void LFpsCamera::_updateCamera()
    {
        m_front.x = cos( glm::radians( m_yaw ) ) * cos( glm::radians( m_pitch ) );
        m_front.y = sin( glm::radians( m_pitch ) );
        m_front.z = sin( glm::radians( m_yaw ) ) * cos( glm::radians( m_pitch ) );
        m_front.normalize();

        m_right = LVec3::cross( m_front, m_worldUp );
        m_right.normalize();

        m_up = LVec3::cross( m_right, m_front );
        m_up.normalize();
    }

    void LFpsCamera::dumpInfo()
    {
        cout << "Camera base information for camera < " << m_cameraId << " > *******" << endl;

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