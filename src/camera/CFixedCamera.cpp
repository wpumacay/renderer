
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

        m_front = ( m_targetPoint - m_position ).normalized();
        m_right = tinymath::cross( m_front, m_worldUp ).normalized();
        m_up    = tinymath::cross( m_right, m_front ).normalized();

        _buildViewMatrix();
    }

    void CFixedCamera::setCameraTransform( const CMat4& transform )
    {
        m_position = CVec3( transform.col( 3 ) );

        m_front = CVec3( transform.col( 0 ) );
        m_right = -CVec3( transform.col( 1 ) );
        m_up = CVec3( transform.col( 2 ) );

        m_targetDir = m_front;

        _buildViewMatrix();
    }

    void CFixedCamera::_positionChangedInternal()
    {
        m_front = ( m_targetPoint - m_position ).normalized();
        m_right = tinymath::cross( m_front, m_worldUp ).normalized();
        m_up    = tinymath::cross( m_right, m_front ).normalized();

        m_targetDir = m_front;
    }

    void CFixedCamera::_targetPointChangedInternal()
    {
        m_front = ( m_targetPoint - m_position ).normalized();
        m_right = tinymath::cross( m_front, m_worldUp ).normalized();
        m_up    = tinymath::cross( m_right, m_front ).normalized();

        m_targetDir = m_front;
    }

    void CFixedCamera::_updateInternal()
    {
        // nothing to do for now
    }

    void CFixedCamera::_resizeInternal( int width, int height )
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