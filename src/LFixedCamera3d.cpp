

#include "LFixedCamera3d.h"


namespace engine
{

    LFixedCamera3d::LFixedCamera3d( const string& name,
                                    const LVec3& pos,
                                    const LVec3& targetPoint,
                                    int worldUpId,
                                    float fov,
                                    float aspectRatio,
                                    float zNear, float zFar ) 
        : LICamera( name, pos, targetPoint, worldUpId, fov, aspectRatio, zNear, zFar )
    {
        m_type = LFixedCamera3d::GetStaticType();

        m_front = LVec3::normalize( m_targetPoint - m_pos );
        m_right = LVec3::normalize( LVec3::cross( m_front, m_worldUp ) );
        m_up    = LVec3::normalize( LVec3::cross( m_right, m_front ) );

        _buildViewMatrix();
    }

    void LFixedCamera3d::update( float dt )
    {
        // Nothing to do here (like the meme xD)
    }
}