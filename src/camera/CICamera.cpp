
#include <camera/CICamera.h>

namespace engine
{

    std::string toString( const eCameraType& type )
    {
        if ( type == eCameraType::BASE ) return "base";
        if ( type == eCameraType::FIXED ) return "fixed";
        if ( type == eCameraType::FPS ) return "fps";
        if ( type == eCameraType::ORBIT ) return "orbit";

        ENGINE_CORE_ASSERT( false, "Unknown camera type given" );

        return "undefined";
    }

    std::string toString( const eCameraProjection& proj )
    {
        if ( proj == eCameraProjection::PERSPECTIVE ) return "perspective";
        if ( proj == eCameraProjection::ORTHOGRAPHIC ) return "orthographic";

        ENGINE_CORE_ASSERT( false, "Unknown projection type given" );

        return "undefined";
    }

    std::string toString( const CCameraProjData& projData )
    {
        std::string _strRep;

        _strRep += engine::toString( projData.projection ) + "\n\r";

        if ( projData.projection == eCameraProjection::PERSPECTIVE )
        {
            _strRep += "fov     : " + std::to_string( projData.fov ) + "\n\r";
            _strRep += "aspect  : " + std::to_string( projData.aspect ) + "\n\r";
        }
        else if ( projData.projection == eCameraProjection::ORTHOGRAPHIC )
        {
            _strRep += "width   : " + std::to_string( projData.width ) + "\n\r";
            _strRep += "height  : " + std::to_string( projData.height ) + "\n\r";
        }

        _strRep += "zNear   : " + std::to_string( projData.zNear ) + "\n\r";
        _strRep += "zFar    : " + std::to_string( projData.zFar ) + "\n\r";

        return _strRep;
    }

    CICamera::CICamera( const std::string& name,
                        const CVec3& position,
                        const CVec3& targetPoint,
                        const eAxis& upAxis,
                        const CCameraProjData& projData )
    {
        m_name          = name;
        m_position      = position;
        m_targetPoint   = targetPoint;
        m_targetDir     = CVec3::normalize( m_targetPoint - m_position );
        m_upAxis        = upAxis;
        m_projData      = projData;
        m_active        = true;
        m_worldUp       = { 0.0f, 0.0f, 1.0f };
        m_type          = eCameraType::BASE;

        if ( m_upAxis == eAxis::X ) m_worldUp = { 1.0f, 0.0f, 0.0f };
        if ( m_upAxis == eAxis::Y ) m_worldUp = { 0.0f, 1.0f, 0.0f };
        if ( m_upAxis == eAxis::Z ) m_worldUp = { 0.0f, 0.0f, 1.0f };

        _buildProjMatrix();
    }

    CICamera::~CICamera()
    {
        // nothing to delete for now
    }

    void CICamera::setPosition( const CVec3& position )
    {
        m_position = position;

        _positionChangedInternal();
        _buildViewMatrix();
    }

    void CICamera::setTargetPoint( const CVec3& targetPoint )
    {
        m_targetPoint = targetPoint;

        _targetPointChangedInternal();
        _buildViewMatrix();
    }

    void CICamera::setActiveMode( bool activeMode )
    {
        m_active = activeMode;
    }

    void CICamera::setProjectionData( const CCameraProjData& projData )
    {
        m_projData = projData;
        _buildProjMatrix();
    }

    void CICamera::update()
    {
        _updateInternal();
    }

    void CICamera::resize( int width, int height )
    {
        m_projData.aspect = ( (float)width ) / height;
        m_projData.width = 10.0f * ( (float)width ) / height;
        m_projData.height = 10.0f;

        _resizeInternal( width, height );

        _buildProjMatrix();
    }

    std::string CICamera::toString()
    {
        std::string _strRep;
        _strRep += "name        : " + m_name + "\n\r";
        _strRep += "position    : " + engine::toString( m_position ) + "\n\r";
        _strRep += "targetDir   : " + engine::toString( m_targetDir ) + "\n\r";
        _strRep += "worldUp     : " + engine::toString( m_worldUp ) + "\n\r";
        _strRep += engine::toString( m_projData );

        _strRep += _toStringInternal();

        return _strRep;
    }

    void CICamera::_buildViewMatrix()
    {
        // View matrix is
        /*
        *  |                |
        *  |     R^T   -R'p |
        *  |                |
        *  | 0   0   0   1  |
        */
        // Also, it's column major, so must keep layout ...
        // [ c1x c1y c1z c1w, c2x c2y c2z c2w, ... ]
        m_matView.buff[0] = m_right.x;
        m_matView.buff[1] = m_up.x;
        m_matView.buff[2] = -m_front.x;
        m_matView.buff[3] = 0;

        m_matView.buff[4] = m_right.y;
        m_matView.buff[5] = m_up.y;
        m_matView.buff[6] = -m_front.y;
        m_matView.buff[7] = 0;

        m_matView.buff[8]  = m_right.z;
        m_matView.buff[9]  = m_up.z;
        m_matView.buff[10] = -m_front.z;
        m_matView.buff[11] = 0;

        m_matView.buff[12] = -CVec3::dot( m_right, m_position );
        m_matView.buff[13] = -CVec3::dot( m_up, m_position );
        m_matView.buff[14] = CVec3::dot( m_front, m_position );
        m_matView.buff[15] = 1;
    }

    void CICamera::_buildProjMatrix()
    {
        if ( m_projData.projection == eCameraProjection::PERSPECTIVE )
        {
            m_matProj = CMat4::perspective( m_projData.fov, 
                                            m_projData.aspect,
                                            m_projData.zNear, 
                                            m_projData.zFar );
        }
        else if ( m_projData.projection == eCameraProjection::ORTHOGRAPHIC )
        {
            m_matProj = CMat4::ortho( m_projData.width,
                                      m_projData.height,
                                      m_projData.zNear,
                                      m_projData.zFar );
        }
    }

}