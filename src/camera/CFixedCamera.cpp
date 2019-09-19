
#include <camera/CFixedCamera.h>

namespace engine
{

    CFixedCamera::CFixedCamera( const std::string& name,
                                const LVec3& position,
                                const LVec3& targetPoint,
                                const eAxis& upAxis,
                                const CCameraProjData& projData )
        : CICamera( name, position, targetPoint, upAxis, projData )
    {
        m_type = eCameraType::FIXED;

        m_front = LVec3::normalize( m_targetPoint - m_position );
        m_right = LVec3::normalize( LVec3::cross( m_front, m_worldUp ) );
        m_up    = LVec3::normalize( LVec3::cross( m_right, m_front ) );

        _buildViewMatrix();
    }

    void CFixedCamera::_positionChangedInternal()
    {
        // keep looking at same target point
        m_front = LVec3::normalize( m_targetPoint - m_position );
        m_right = LVec3::normalize( LVec3::cross( m_front, m_worldUp ) );
        m_up    = LVec3::normalize( LVec3::cross( m_right, m_front ) );

        m_targetDir = m_front;
    }

    void CFixedCamera::_updateInternal()
    {
        // nothing to do for now
    }

    std::string CFixedCamera::_toStringInternal()
    {
        std::string _strRep;

        _strRep += "front   : " + m_front.toString() + "\n\r";
        _strRep += "right   : " + m_right.toString() + "\n\r";
        _strRep += "up      : " + m_up.toString() + "\n\r";

        return _strRep;
    }

}