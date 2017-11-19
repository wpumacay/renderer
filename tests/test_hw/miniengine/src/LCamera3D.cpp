

#include "../include/LCamera3D.h"

/**
* Implementation based on this tutorial's camera class
* https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h
*/


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
    }



}