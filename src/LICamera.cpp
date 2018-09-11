
#include "LICamera.h"




namespace engine
{


    LICamera::LICamera( const LVec3& pos,
                        const LVec3& targetDir,
                        const LVec3& worldUp,
                        float fov,
                        float aspectRatio,
                        float zNear, float zFar )
    {
        m_pos = pos;
        m_targetDir = targetDir;
        m_worldUp = worldUp;

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