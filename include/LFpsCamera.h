
#pragma once

#include "LICamera.h"
#include <input/LInputSystem.h>

#define CAM_DEFAULT_YAW -90.0f
#define CAM_DEFAULT_PITCH 0.0f
#define CAM_DEFAULT_FOV 45.0f
#define CAM_DEFAULT_SENSITIVITY 0.25f
#define CAM_DEFAULT_SPEED 5.0f
#define CAM_MAX_DELTA 10

using namespace std;

namespace engine
{


    class LFpsCamera : public LICamera
    {

        private :

        float m_roll;
        float m_pitch;
        float m_yaw;

        float m_rAngRot;// small delta around the camera's right axis
        float m_uAngRot;// small delta around the camera's up axis

        float m_sensitivity;

        LVec3 m_speed;
        float m_baseSpeed;

        float m_lastX;
        float m_lastY;

        protected :

        void _updateCameraVectors();

        public :

        LFpsCamera( const string& name,
                    const LVec3& pos,
                    const LVec3& targetPoint,
                    int worldUpId,
                    float fov = 45.0f,
                    float aspectRatio = ( (float)APP_WIDTH ) / APP_HEIGHT,
                    float zNear = 0.1f, float zFar = 100.0f );

        static string GetStaticType() { return string( "fps" ); }

        void update( float dt ) override;

        void dumpInfo() override;
    };




}