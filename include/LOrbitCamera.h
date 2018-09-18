
#pragma once

#include <LICamera.h>
#include <input/LInputSystem.h>



namespace engine
{

    class LOrbitCamera : public LICamera
    {

        private :

        // spherical coordinates
        float m_rho;
        float m_theta;
        float m_phi;

        // reference sphericals used when dragging
        float m_rho0;
        float m_theta0;
        float m_phi0;

        // relative position vector
        LVec3 m_r;
        // 

        public :

        enum OrbitCameraState
        {
            IDLE = 0,
            DRAGGING = 1,
            MOVING_TARGET = 2
        };

        LOrbitCamera( const string& name,
                      const LVec3& pos,
                      const LVec3& targetPoint,
                      int worldUpId,
                      float fov = 45.0f,
                      float aspectRatio = ( (float)APP_WIDTH ) / APP_HEIGHT,
                      float zNear = 10.0f, float zFar = 40.0f );
        

    };

}