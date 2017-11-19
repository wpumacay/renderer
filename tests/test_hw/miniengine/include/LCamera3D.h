
#include "../LMiniCommon.h"
#include <glm/gtc/matrix_transform.hpp>

#pragma once

#define CAM_DEFAULT_YAW -90.0f
#define CAM_DEFAULT_PITCH 0.0f
#define CAM_DEFAULT_FOV 45.0f
#define CAM_DEFAULT_SENSITIVITY 0.1f


namespace miniengine
{


        class LCamera3D
        {

            protected :

            // Camera vectors, only updateable by euler angles
            LVec3 m_front;
            LVec3 m_up;
            LVec3 m_right;

            // GLobal world vector
            LVec3 m_worldUp;

            void _updateCameraSystem();

            public :

            // Field of view in degrees
            float fov;
            // Position vector
            LVec3 pos;
            // Euler angles -> x : roll, y : pitch, z : yaw
            LVec3 euler;

            float sensitivity;

            LCamera3D();
            LCamera3D( const LVec3& pPos, const LVec3& pWorldUp );

            virtual glm::mat4 viewMatrix();

            virtual void update( float dt );
        };






}