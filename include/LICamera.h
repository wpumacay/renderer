

#pragma once

#include "LCommon.h"
#include "../Config.h"

using namespace std;

namespace engine
{

    class LICamera
    {
        protected :

        LVec3 m_pos;
        LVec3 m_targetDir;
        LVec3 m_worldUp;

        string m_type;

        float m_fov;
        float m_aspectRatio;
        float m_zNear;
        float m_zFar;

        string m_cameraId;

        virtual void _updateCamera();

        public :

        LICamera( const LVec3& pos,
                  const LVec3& targetDir,
                  const LVec3& worldUp,
                  float fov = 45.0f,
                  float aspectRatio = ( (float)APP_WIDTH ) / APP_HEIGHT,
                  float zNear = 10.0f, float zFar = 40.0f );

        ~LICamera();

        static string getStaticType() { return string( "base" ); }
        string type() { return m_type; }

        void setTargetDir( const LVec3& target ) { m_targetDir = target; }
        LVec3 getTargetDir() const { return m_targetDir; }

        void setPosition( const LVec3& pos );
        LVec3 getPosition() const { return m_pos; }

        float getFov() { return m_fov; }
        float getZNear() { return m_zNear; }
        float getZFar() { return m_zFar; }

        virtual glm::mat4 getViewMatrix() = 0;
        glm::mat4 getProjectionMatrix();

        virtual void update( float dt ) 
        {
            // Override this
        }

        virtual void dumpInfo()
        {
            cout << "Camera base information for camera < " << m_cameraId << " > *******" << endl;

            cout << "pos: " << m_pos.toString() << endl;
            cout << "targetDir: " << m_targetDir.toString() << endl;
            cout << "worldUp: " << m_worldUp.toString() << endl;
            cout << "fov: " << m_fov << endl;
            cout << "aspectRatio: " << m_aspectRatio << endl;
            cout << "zNear: " << m_zNear << endl;
            cout << "zFar: " << m_zFar << endl;

            cout << "******************************************" << endl;
        }

    };



}