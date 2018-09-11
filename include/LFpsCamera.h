
#pragma once

#include "LICamera.h"
#include "LInputHandler.h"

#define CAM_DEFAULT_YAW -90.0f
#define CAM_DEFAULT_PITCH 0.0f
#define CAM_DEFAULT_FOV 45.0f
#define CAM_DEFAULT_SENSITIVITY 0.1f
#define CAM_DEFAULT_SPEED 5.0f
#define CAM_MAX_DELTA 20

using namespace std;

namespace engine
{


    class LFpsCamera : public LICamera
    {

        private :

        float m_roll;
        float m_pitch;
        float m_yaw;

        float m_sensitivity;

        LVec3 m_front;
        LVec3 m_up;
        LVec3 m_right;

        LVec3 m_speed;
        float m_baseSpeed;

        float m_lastX;
        float m_lastY;

        protected :

        void _updateCamera() override;

        public :

        LFpsCamera( const LVec3& pos,
                    const LVec3& worldUp,
                    float fov = 45.0f,
                    float aspectRatio = ( (float)APP_WIDTH ) / APP_HEIGHT,
                    float zNear = 0.1f, float zFar = 100.0f );

        static string getStaticType() { return string( "fps" ); }

        glm::mat4 getViewMatrix() override;

        void update( float dt ) override;

        void dumpInfo() override;
    };




}