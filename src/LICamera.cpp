
#include <LICamera.h>

using namespace std;

namespace engine
{


    LICamera::LICamera( const string& name,
                        const LVec3& pos,
                        const LVec3& targetPoint,
                        int worldUpId,
                        float fov,
                        float aspectRatio,
                        float zNear, float zFar )
    {
        m_name = name;
        m_pos = pos;
        m_targetPoint = targetPoint;
        m_targetDir = LVec3::normalize( m_targetPoint - m_pos );
        m_worldUpVectorId = worldUpId;

        if ( worldUpId == UP_X ) 
        { 
            m_worldUp = LVec3( 1.0f, 0.0f, 0.0f ); 
        }
        else if ( worldUpId == UP_Y ) 
        { 
            m_worldUp = LVec3( 0.0f, 1.0f, 0.0f );
        }
        else if ( worldUpId == UP_Z ) 
        { 
            m_worldUp = LVec3( 0.0f, 0.0f, 1.0f ); 
        }
        else 
        { 
            m_worldUp = LVec3( 0.0f, 1.0f, 0.0f ); 
            m_worldUpVectorId = UP_Y; 
        }

        m_fov = fov;
        m_aspectRatio = aspectRatio;
        m_zNear = zNear;
        m_zFar = zFar;

        m_isActive = true;

        m_projMat = LMat4::perspective( m_fov, m_aspectRatio,
                                        m_zNear, m_zFar );
    }

    LICamera::~LICamera()
    {

    }

    void LICamera::setPosition( const LVec3& pos )
    {
        m_pos = pos;
        _buildViewMatrix();
    }

    void LICamera::_buildViewMatrix()
    {
        // View matrix is ...
        /*
        *  |                |
        *  |     R^T   -R'p |
        *  |                |
        *  | 0   0   0   1  |
        */
        // Also, it's column major, so must keep layout ...
        // [ c1x c1y c1z c1w, c2x c2y c2z c2w, ... ]
        m_viewMat.buff[0] = m_right.x;
        m_viewMat.buff[1] = m_up.x;
        m_viewMat.buff[2] = -m_front.x;
        m_viewMat.buff[3] = 0;

        m_viewMat.buff[4] = m_right.y;
        m_viewMat.buff[5] = m_up.y;
        m_viewMat.buff[6] = -m_front.y;
        m_viewMat.buff[7] = 0;

        m_viewMat.buff[8]  = m_right.z;
        m_viewMat.buff[9]  = m_up.z;
        m_viewMat.buff[10] = -m_front.z;
        m_viewMat.buff[11] = 0;

        m_viewMat.buff[12] = -LVec3::dot( m_right, m_pos );
        m_viewMat.buff[13] = -LVec3::dot( m_up, m_pos );
        m_viewMat.buff[14] = LVec3::dot( m_front, m_pos );
        m_viewMat.buff[15] = 1;

        // std::cout << "vectors" << std::endl;
        // std::cout << m_front.toString() << std::endl;
        // std::cout << m_right.toString() << std::endl;
        // std::cout << m_up.toString() << std::endl;
        // std::cout << "mat4x4" << std::endl;
        // std::cout << m_viewMat.toString() << std::endl;
    }
}