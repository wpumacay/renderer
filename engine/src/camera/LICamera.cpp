
#include "../../include/camera/LICamera.h"


namespace engine
{


    LICamera::LICamera( const LVec3& pos,
                        const LVec3& targetDir,
                        const LVec3& worldUp )
    {
        m_pos = pos;
        m_targetDir = targetDir;
        m_worldUp = worldUp;
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
}