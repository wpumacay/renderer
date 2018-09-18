

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

        dumpInfo();
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
            _applyConstraints();
        }
    }

    void LFollowCamera::_applyConstraints()
    {
        if ( m_meshRef == NULL )
        {
            return;
        }

        // delta for the camera position
        LVec3 _h = m_meshRef->pos - m_pos;
        LVec3 _delta = _h - m_targetDir * ( LVec3::dot( _h, m_targetDir ) );
        // apply constraint to camera position
        m_pos = m_pos + _delta;
        // compute current distance
        m_distToRef = ( m_meshRef->pos - m_pos ).length();

        // std::cout << "******************" << std::endl;
        // std::cout << "mesh pos: " << m_meshRef->pos.toString() << std::endl;
        // std::cout << "pos: " << m_pos.toString() << std::endl;
        // std::cout << "delta: " << _delta.toString() << std::endl;
        // std::cout << "******************" << std::endl;
    }

    glm::mat4 LFollowCamera::getViewMatrix()
    {
        glm::vec3 _cameraPos( m_pos.x, m_pos.y, m_pos.z );
        glm::vec3 _cameraDir( m_targetDir.x, m_targetDir.y, m_targetDir.z );
        glm::vec3 _cameraTarget = _cameraPos + _cameraDir;
        glm::vec3 _worldUp( m_worldUp.x, m_worldUp.y, m_worldUp.z );

        return glm::lookAt( _cameraPos, _cameraTarget, _worldUp );
    }

    void LFollowCamera::update( float dt )
    {
        if ( m_meshRef == NULL )
        {
            return;
        }
        _applyConstraints();
    }
}