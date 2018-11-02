
#pragma once

#include <LMesh.h>
#include <LICamera.h>

namespace engine
{

    class LFollowCamera : public LICamera
    {
        private :

        LMesh* m_meshRef;
        float m_distToRef;

        public :

        LFollowCamera( const string& name,
                       const LVec3& pos,
                       const LVec3& targetPoint,
                       int worldUpId,
                       float fov = 45.0f,
                       float aspectRatio = ( (float)APP_WIDTH ) / APP_HEIGHT,
                       float zNear = 0.1f, float zFar = 100.0f );
        ~LFollowCamera();

        static string GetStaticType() { return string( "follow" ); }

        void setMeshReference( LMesh* pMesh );
        void update( float dt ) override;

    };
}