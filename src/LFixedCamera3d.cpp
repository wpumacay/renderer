

#include "LFixedCamera3d.h"


namespace engine
{

    LFixedCamera3d::LFixedCamera3d( const LVec3& pos,
                                    const LVec3& targetDir,
                                    const LVec3& worldUp,
                                    float fov,
                                    float aspectRatio,
                                    float zNear, float zFar ) 
        : LICamera( pos, targetDir, worldUp, fov, aspectRatio, zNear, zFar )
    {
        m_movSpeed = 0.0f;
        m_movDirection = LVec3( 0, 0, 0 );

        m_type = LFixedCamera3d::getStaticType();
    }

    glm::mat4 LFixedCamera3d::getViewMatrix()
    {
        glm::vec3 _cameraPos( m_pos.x, m_pos.y, m_pos.z );
        glm::vec3 _cameraDir( m_targetDir.x, m_targetDir.y, m_targetDir.z );
        glm::vec3 _cameraTarget = _cameraPos + _cameraDir;
        glm::vec3 _worldUp( m_worldUp.x, m_worldUp.y, m_worldUp.z );

#if 0
        glm::vec3 _frustumPointsClipSpace[8] = {
            // near plane
            { -1.0f, -1.0f, -1.0f },{ 1.0f, -1.0f, -1.0f },
            { 1.0f,  1.0f, -1.0f },{ -1.0f,  1.0f, -1.0f },
            // far plane
            { -1.0f, -1.0f,  1.0f },{ 1.0f, -1.0f,  1.0f },
            { 1.0f,  1.0f,  1.0f },{ -1.0f,  1.0f,  1.0f }
        };

        glm::mat4 _viewMatrix = glm::lookAt( _cameraPos, _cameraTarget, _worldUp );
        glm::mat4 _projMatrix = glm::perspective( glm::radians( m_fov ),
                                                  m_aspectRatio, m_zNear, m_zFar );

        glm::mat4 _clipMatrix = _projMatrix * _viewMatrix;
        glm::mat4 _invClipMatrix = glm::inverse( _clipMatrix );

        vector< glm::vec3 > _planes;

        for ( int q = 0; q < 8; q++ )
        {
            glm::vec4 _pointFrustum = _invClipMatrix * glm::vec4( _frustumPointsClipSpace[q], 1.0f );
            glm::vec3 _pointFrustumNormalized = glm::vec3( _pointFrustum.x / _pointFrustum.w,
                                                           _pointFrustum.y / _pointFrustum.w,
                                                           _pointFrustum.z / _pointFrustum.w );

            _planes.push_back( _pointFrustumNormalized );
        }

        cout << "near plane ****" << endl;

        for ( int q = 0; q < 4; q++ )
        {
            cout << "x: " << _planes[q].x << " - y : " << _planes[q].y << " - z: " << _planes[q].z << endl;
        }

        cout << "far plane ****" << endl;

        for ( int q = 4; q < 8; q++ )
        {
            cout << "x: " << _planes[q].x << " - y : " << _planes[q].y << " - z: " << _planes[q].z << endl;
        }

        cout << "**************" << endl;


#endif
        return glm::lookAt( _cameraPos, _cameraTarget, _worldUp );
    }

    void LFixedCamera3d::update( float dt )
    {
        m_pos.x += m_movDirection.x * m_movSpeed * dt;
        m_pos.y += m_movDirection.y * m_movSpeed * dt;
        m_pos.z += m_movDirection.z * m_movSpeed * dt;
    }
}