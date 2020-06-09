
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
        m_Type = eCameraType::FIXED;

        m_Front = ( m_TargetPoint - m_Position ).normalized();
        m_Right = tinymath::cross( m_Front, m_WorldUp ).normalized();
        m_Up    = tinymath::cross( m_Right, m_Front ).normalized();
        _BuildViewMatrix();
    }

    void CFixedCamera::SetCameraTransform( const CMat4& transform )
    {
        m_Position = CVec3( transform.col( 3 ) );

        m_Front = CVec3( transform.col( 0 ) );
        m_Right = -CVec3( transform.col( 1 ) );
        m_Up    = CVec3( transform.col( 2 ) );

        m_TargetDir = m_Front;
        _BuildViewMatrix();
    }

    void CFixedCamera::_PositionChangedInternal()
    {
        m_Front = ( m_TargetPoint - m_Position ).normalized();
        m_Right = tinymath::cross( m_Front, m_WorldUp ).normalized();
        m_Up    = tinymath::cross( m_Right, m_Front ).normalized();

        m_TargetDir = m_Front;
    }

    void CFixedCamera::_TargetPointChangedInternal()
    {
        m_Front = ( m_TargetPoint - m_Position ).normalized();
        m_Right = tinymath::cross( m_Front, m_WorldUp ).normalized();
        m_Up    = tinymath::cross( m_Right, m_Front ).normalized();

        m_TargetDir = m_Front;
    }

    std::string CFixedCamera::_ToStringInternal() const
    {
        std::string strrep;
        strrep += "front   : " + engine::toString( m_Front ) + "\n\r";
        strrep += "right   : " + engine::toString( m_Right ) + "\n\r";
        strrep += "up      : " + engine::toString( m_Up ) + "\n\r";
        return strrep;
    }

}