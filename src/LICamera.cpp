
#include "LICamera.h"

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
    }

    LICamera::~LICamera()
    {

    }

    void LICamera::setPosition( const LVec3& pos )
    {
        m_pos = pos;
        _updateCamera();
    }

    void LICamera::_updateCamera()
    {
        // Override this
    }

    glm::mat4 LICamera::getProjectionMatrix()
    {
        return glm::perspective( glm::radians( m_fov ),
                                 m_aspectRatio, m_zNear, m_zFar );
    }
}