
#pragma once

#include "LICamera.h"

using namespace std;

namespace engine
{


    class LFixedCamera3d : public LICamera
    {
        private :

        float m_movSpeed;
        LVec3 m_movDirection;

        public :

        LFixedCamera3d( const LVec3& pos,
                        const LVec3& targetDir,
                        const LVec3& worldUp,
                        float fov = 45.0f,
                        float aspectRatio = ( (float)APP_WIDTH ) / APP_HEIGHT,
                        float zNear = 1.0f, float zFar = 20.0f );

        static string getStaticType() { return string( "fixec3d" ); }

        glm::mat4 getViewMatrix() override;

        void update( float dt ) override;

        void setMovVelocity( float speed, const LVec3& direction )
        {
            m_movSpeed = speed;
            m_movDirection = direction;
        }
    };




}