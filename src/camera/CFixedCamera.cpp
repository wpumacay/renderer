
#include <camera/CFixedCamera.h>

namespace engine
{

    CFixedCamera::CFixedCamera( const std::string& name,
                                const CVec3& position,
                                const CVec3& targetPoint,
                                const eAxis& upAxis,
                                const CCameraProjData& projData )
        : CICamera( name, position, targetPoint, upAxis, projData )
    {
        m_type = eCameraType::FIXED;

        m_front = CVec3::normalize( m_targetPoint - m_position );
        m_right = CVec3::normalize( CVec3::cross( m_front, m_worldUp ) );
        m_up    = CVec3::normalize( CVec3::cross( m_right, m_front ) );

        _buildViewMatrix();
    }

    void CFixedCamera::_positionChangedInternal()
    {
        // keep looking at same target point
        m_front = CVec3::normalize( m_targetPoint - m_position );
        m_right = CVec3::normalize( CVec3::cross( m_front, m_worldUp ) );
        m_up    = CVec3::normalize( CVec3::cross( m_right, m_front ) );

        m_targetDir = m_front;
    }

    void CFixedCamera::_updateInternal()
    {
        // nothing to do for now
    }

    std::string CFixedCamera::_toStringInternal()
    {
        std::string _strRep;

        _strRep += "front   : " + engine::toString( m_front ) + "\n\r";
        _strRep += "right   : " + engine::toString( m_right ) + "\n\r";
        _strRep += "up      : " + engine::toString( m_up ) + "\n\r";

        return _strRep;
    }

}