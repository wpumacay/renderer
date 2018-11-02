

#include <LFollowCamera.h>


namespace engine
{

    LFollowCamera::LFollowCamera( const string& name,
                                  const LVec3& pos,
                                  const LVec3& targetPoint,
                                  int worldUpId,
                                  float fov, float aspectRatio,
                                  float zNear, float zFar )
        : LICamera( name, pos, targetPoint, worldUpId, fov, aspectRatio, zNear, zFar )
    {
        m_meshRef = NULL;
        m_distToRef = 0.0f;

        m_type = LFollowCamera::GetStaticType();

        _buildViewMatrix();
    }

    LFollowCamera::~LFollowCamera()
    {
        m_meshRef = NULL;
    }

    void LFollowCamera::setMeshReference( LMesh* pMesh )
    {
        m_meshRef = pMesh;
        if ( m_meshRef != NULL )
        {
            update( 0.0f );
        }
    }

    void LFollowCamera::update( float dt )
    {
        if ( m_meshRef == NULL )
        {
            return;
        }

        LVec3 _h = m_meshRef->pos - m_pos;
        LVec3 _delta = _h - m_targetDir * ( LVec3::dot( _h, m_targetDir ) );
        // apply constraint to camera position
        m_pos = m_pos + _delta;
        // compute current distance
        m_distToRef = ( m_meshRef->pos - m_pos ).length();

        _buildViewMatrix();
    }
}