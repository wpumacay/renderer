
#include <camera/CICamera.h>

namespace engine
{
    std::string ToString( const eCameraType& type )
    {
        /**/ if ( type == eCameraType::FIXED )  return "fixed";
        else if ( type == eCameraType::FPS )    return "fps";
        else if ( type == eCameraType::ORBIT )  return "orbit";

        ENGINE_CORE_CRITICAL( "Undefined camera type-enum" );
        return "undefined";
    }

    std::string ToString( const eCameraProjection& proj )
    {
        /**/ if ( proj == eCameraProjection::PERSPECTIVE )  return "perspective";
        else if ( proj == eCameraProjection::ORTHOGRAPHIC ) return "orthographic";

        ENGINE_CORE_CRITICAL( "Undefined camera projection type-enum" );
        return "undefined";
    }

    std::string ToString( const CCameraProjData& proj_data )
    {
        std::string strrep;
        strrep += engine::ToString( proj_data.projection ) + "\n\r";
        if ( proj_data.projection == eCameraProjection::PERSPECTIVE )
        {
            strrep += "fov     : " + std::to_string( proj_data.fov ) + "\n\r";
            strrep += "aspect  : " + std::to_string( proj_data.aspect ) + "\n\r";
        }
        else if ( proj_data.projection == eCameraProjection::ORTHOGRAPHIC )
        {
            strrep += "width   : " + std::to_string( proj_data.width ) + "\n\r";
            strrep += "height  : " + std::to_string( proj_data.height ) + "\n\r";
        }
        strrep += "zNear   : " + std::to_string( proj_data.zNear ) + "\n\r";
        strrep += "zFar    : " + std::to_string( proj_data.zFar ) + "\n\r";
        strrep += "vpwidth : " + std::to_string( proj_data.viewportWidth ) + "\n\r";
        strrep += "vpheight: " + std::to_string( proj_data.viewportHeight ) + "\n\r";
        return strrep;
    }

    CICamera::CICamera( const std::string& name,
                        const CVec3& position,
                        const CVec3& target_point,
                        const eAxis& up_axis,
                        const CCameraProjData& proj_data )
    {
        m_Name          = name;
        m_Position      = position;
        m_TargetPoint   = target_point;
        m_TargetDir     = ( m_TargetPoint - m_Position ).normalized();
        m_UpAxis        = up_axis;
        m_ProjData      = proj_data;
        m_Active        = true;
        m_WorldUp       = { 0.0f, 0.0f, 1.0f };

        /**/ if ( m_UpAxis == eAxis::X ) m_WorldUp = { 1.0f, 0.0f, 0.0f };
        else if ( m_UpAxis == eAxis::Y ) m_WorldUp = { 0.0f, 1.0f, 0.0f };
        else if ( m_UpAxis == eAxis::Z ) m_WorldUp = { 0.0f, 0.0f, 1.0f };
        _BuildProjMatrix();
    }

    CICamera::~CICamera()
    {
        // Nothing to delete for now
    }

    void CICamera::SetPosition( const CVec3& position )
    {
        m_Position = position;
        _PositionChangedInternal();
        _BuildViewMatrix();
    }

    void CICamera::SetTargetPoint( const CVec3& target_point )
    {
        m_TargetPoint = target_point;

        _TargetPointChangedInternal();
        _BuildViewMatrix();
    }

    void CICamera::SetActiveMode( bool activeMode )
    {
        m_Active = activeMode;
    }

    void CICamera::SetProjectionData( const CCameraProjData& proj_data )
    {
        m_ProjData = proj_data;
        _BuildProjMatrix();
    }

    void CICamera::Update()
    {
        _UpdateInternal();
    }

    void CICamera::Resize( int width, int height )
    {
        m_ProjData.aspect = ( (float)width ) / height;
        m_ProjData.width = 10.0f * ( (float)width ) / height;
        m_ProjData.height = 10.0f;
        m_ProjData.viewportWidth = (float)width;
        m_ProjData.viewportHeight = (float)height;
        _ResizeInternal( width, height );
        _BuildProjMatrix();
    }

    std::string CICamera::ToString() const
    {
        std::string strrep;
        strrep += "name        : " + m_Name + "\n\r";
        strrep += "position    : " + engine::toString( m_Position ) + "\n\r";
        strrep += "targetDir   : " + engine::toString( m_TargetDir ) + "\n\r";
        strrep += "worldUp     : " + engine::toString( m_WorldUp ) + "\n\r";
        strrep += engine::ToString( m_ProjData );
        strrep += _ToStringInternal();
        return strrep;
    }

    void CICamera::_BuildViewMatrix()
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
        m_MatView( 0, 0 ) = m_Right.x();
        m_MatView( 1, 0 ) = m_Up.x();
        m_MatView( 2, 0 ) = -m_Front.x();
        m_MatView( 3, 0 ) = 0;

        m_MatView( 0, 1 ) = m_Right.y();
        m_MatView( 1, 1 ) = m_Up.y();
        m_MatView( 2, 1 ) = -m_Front.y();
        m_MatView( 3, 1 ) = 0;

        m_MatView( 0, 2 ) = m_Right.z();
        m_MatView( 1, 2 ) = m_Up.z();
        m_MatView( 2, 2 ) = -m_Front.z();
        m_MatView( 3, 2 ) = 0;

        m_MatView( 0, 3 ) = -m_Right.dot( m_Position );
        m_MatView( 1, 3 ) = -m_Up.dot( m_Position );
        m_MatView( 2, 3 ) = m_Front.dot( m_Position );
        m_MatView( 3, 3 ) = 1;
    }

    void CICamera::_BuildProjMatrix()
    {
        if ( m_ProjData.projection == eCameraProjection::PERSPECTIVE )
            m_MatProj = engine::perspective( m_ProjData.fov, m_ProjData.aspect, m_ProjData.zNear, m_ProjData.zFar );
        else if ( m_ProjData.projection == eCameraProjection::ORTHOGRAPHIC )
            m_MatProj = engine::ortho( m_ProjData.width, m_ProjData.height, m_ProjData.zNear, m_ProjData.zFar );
    }
}
