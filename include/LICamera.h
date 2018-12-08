

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
        LVec3 m_worldUp;

        LVec3 m_targetPoint;
        LVec3 m_targetDir;
        
        LVec3 m_front;
        LVec3 m_up;
        LVec3 m_right;
        
        int m_worldUpVectorId;
        LMat4 m_viewMat;
        LMat4 m_projMat;

        string m_type;

        float m_fov;
        float m_aspectRatio;
        float m_zNear;
        float m_zFar;

        bool m_isActive;

        string m_name;

        void _buildViewMatrix();

        public :

        enum WorldUpVector
        {
            UP_X = 0,
            UP_Y = 1,
            UP_Z = 2
        };

        LICamera( const string& name,
                  const LVec3& pos,
                  const LVec3& targetPoint,
                  int worldUpId,
                  float fov = 45.0f,
                  float aspectRatio = ( (float)APP_WIDTH ) / APP_HEIGHT,
                  float zNear = 0.1f, float zFar = 100.0f );

        ~LICamera();

        static string GetStaticType() { return string( "base" ); }
        string type() { return m_type; }
        string name() { return m_name; }

        LVec3 getTargetPoint() const { return m_targetPoint; }
        LVec3 getTargetDir() const { return m_targetDir; }

        void setPosition( const LVec3& pos );
        LVec3 getPosition() const { return m_pos; }

        float getFov() { return m_fov; }
        float getZNear() { return m_zNear; }
        float getZFar() { return m_zFar; }

        LMat4 getViewMatrix() { return m_viewMat; }
        LMat4 getProjectionMatrix() { return m_projMat; }

        virtual void update( float dt ) = 0;

        virtual void dumpInfo()
        {
            cout << "Camera base information for camera < " << m_name << " > *******" << endl;

            cout << "pos: " << m_pos.toString() << endl;
            cout << "targetDir: " << m_targetDir.toString() << endl;
            cout << "worldUp: " << m_worldUp.toString() << endl;
            cout << "fov: " << m_fov << endl;
            cout << "aspectRatio: " << m_aspectRatio << endl;
            cout << "zNear: " << m_zNear << endl;
            cout << "zFar: " << m_zFar << endl;

            cout << "******************************************" << endl;
        }

        void setActiveMode( bool activeMode ) { m_isActive = activeMode; }
        bool isActive() { return m_isActive; }
    };



}