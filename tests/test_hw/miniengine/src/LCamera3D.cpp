

#include "../include/LCamera3D.h"

/**
* Implementation based on this tutorial's camera class
* https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h
*/

#include <GLFW/glfw3.h>
#include <LConfig.h>

namespace miniengine
{


    LCamera3D::LCamera3D()
    {
        fov = CAM_DEFAULT_FOV;

        euler.x = 0.0f;
        euler.y = CAM_DEFAULT_PITCH;
        euler.z = CAM_DEFAULT_YAW;

        sensitivity = CAM_DEFAULT_SENSITIVITY;

        pos = LVec3( 0.0f, 0.0f, 0.0f );

        m_front = LVec3( 0.0f, 0.0f, -1.0f );
        m_worldUp = LVec3( 0.0f, 1.0f, 0.0f );

        m_firstMouseMove = true;

        m_baseSpeed = CAM_DEFAULT_SPEED;

        _updateCameraSystem();
    }


    LCamera3D::LCamera3D( const LVec3& pPos, const LVec3& pWorldUp )
    {
        fov = CAM_DEFAULT_FOV;

        euler.x = 0.0f;
        euler.y = CAM_DEFAULT_PITCH;
        euler.z = CAM_DEFAULT_YAW;

        sensitivity = CAM_DEFAULT_SENSITIVITY;

        pos = pPos;

        m_front = LVec3( 0.0f, 0.0f, -1.0f );
        m_worldUp = pWorldUp;

        m_firstMouseMove = true;

        m_baseSpeed = CAM_DEFAULT_SPEED;

        _updateCameraSystem();
    }

    void LCamera3D::_updateCameraSystem()
    {
        m_front.x = cos( glm::radians( euler.z ) ) * cos( glm::radians( euler.y ) );
        m_front.y = sin( glm::radians( euler.y ) );
        m_front.z = sin( glm::radians( euler.z ) ) * cos( glm::radians( euler.y ) );
        m_front.normalize();

        m_right = LVec3::cross( m_front, m_worldUp );
        m_right.normalize();

        m_up = LVec3::cross( m_right, m_front );
        m_up.normalize();
    }

    glm::mat4 LCamera3D::viewMatrix()
    {
        return glm::lookAt( glm::vec3( pos.x, pos.y, pos.z ),
                            glm::vec3( pos.x + m_front.x, 
                                       pos.y + m_front.y,
                                       pos.z + m_front.z ),
                            glm::vec3( m_up.x, m_up.y, m_up.z ) );
    }


    void LCamera3D::update( float dt )
    {
        // TODO: Implement this part for camera movement and more ...
        // interesting stuff

        LVec3 _dFront = m_front * ( m_speed.x * dt );
        LVec3 _dRight = m_right * ( m_speed.z * dt );

        pos = pos + _dFront + _dRight;
    }

    void LCamera3D::onKeyDown( int pKey )
    {
        if ( pKey == GLFW_KEY_W )
        {
            m_speed.x = m_baseSpeed;
        }
        else if ( pKey == GLFW_KEY_S )
        {
            m_speed.x = -m_baseSpeed;
        }
        else if ( pKey == GLFW_KEY_D )
        {
            m_speed.z = m_baseSpeed;
        }
        else if ( pKey == GLFW_KEY_A )
        {
            m_speed.z = -m_baseSpeed;
        }
    }

    void LCamera3D::onKeyUp( int pKey )
    {
        if ( pKey == GLFW_KEY_W ||
             pKey == GLFW_KEY_S )
        {
            m_speed.x = 0.0f;
        }
        else if ( pKey == GLFW_KEY_D ||
                  pKey == GLFW_KEY_A )
        {
            m_speed.z = 0.0f;
        }
    }

    void LCamera3D::onMouseDown( float x, float y )
    {
        m_lastX = x;
        m_lastY = y;
    }

    void LCamera3D::onMouseUp( float x, float y )
    {
        m_lastX = x;
        m_lastY = y;
    }

    void LCamera3D::onMouseMove( double x, double y )
    {
        //x -= ENGINE_APP_WIDTH / 2;
        //y -= ENGINE_APP_HEIGHT / 2;

        if ( m_firstMouseMove )
        {
            cout << "first check!!!" << endl;
            cout << "euler: " << euler.x << " - " << euler.y << " - " << euler.z << endl;

            m_lastX = x;
            m_lastY = y;

            m_firstMouseMove = false;
        }

        float _xOff = x - m_lastX;
        float _yOff = m_lastY - y;

        m_lastX = x;
        m_lastY = y;

        _xOff *= sensitivity;
        _yOff *= sensitivity;

        euler.y += _yOff;
        euler.z += _xOff;

        euler.y = min( max( euler.y, -89.0f ), 89.0f );

        //cout << "euler: " << euler.x << " - " << euler.y << " - " << euler.z << endl;
        //cout << "x: " << x << " - y: " << y << endl;

        _updateCameraSystem();
    }

}