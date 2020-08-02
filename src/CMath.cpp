// @todo: move all helpers to tiny_math (should be templated)

#include <CMath.h>

namespace engine
{

    std::string ToString( const CVec2& v )
    {
        std::string strrep;
        strrep += "( ";  strrep += std::to_string( v.x() );
        strrep += " , "; strrep += std::to_string( v.y() );
        strrep += " )";
        return strrep;
    }

    std::string ToString( const CVec3& v )
    {
        std::string strrep;
        strrep += "( ";  strrep += std::to_string( v.x() );
        strrep += " , "; strrep += std::to_string( v.y() );
        strrep += " , "; strrep += std::to_string( v.z() );
        strrep += " )";
        return strrep;
    }

    std::string ToString( const CVec4& v )
    {
        std::string strrep;
        strrep += "( ";  strrep += std::to_string( v.x() );
        strrep += " , "; strrep += std::to_string( v.y() );
        strrep += " , "; strrep += std::to_string( v.z() );
        strrep += " , "; strrep += std::to_string( v.w() );
        strrep += " )";
        return strrep;
    }

    std::string ToString( const CMat2& m )
    {
        std::string strrep;
        strrep += "[ ";
        for ( int i = 0; i < 2; i++ )
        {
            strrep += "[ ";
            strrep += std::to_string( m( i, 0 ) ); strrep += ", ";
            strrep += std::to_string( m( i, 1 ) );
            strrep += ( i != 1 ) ? " ]\n\r  " : " ]";
        }
        strrep += " ]";
        return strrep;
    }

    std::string ToString( const CMat3& m )
    {
        std::string strrep;

        strrep += "[ ";
        for ( int i = 0; i < 3; i++ )
        {
            strrep += "[ ";
            strrep += std::to_string( m( i, 0 ) ); strrep += ", ";
            strrep += std::to_string( m( i, 1 ) ); strrep += ", ";
            strrep += std::to_string( m( i, 2 ) );
            strrep += ( i != 2 ) ? " ]\n\r  " : " ]";
        }
        strrep += " ]";

        return strrep;
    }

    std::string ToString( const CMat4& m )
    {
        std::string strrep;

        strrep += "[ ";
        for ( int i = 0; i < 4; i++ )
        {
            strrep += "[ ";
            strrep += std::to_string( m( i, 0 ) ); strrep += ", ";
            strrep += std::to_string( m( i, 1 ) ); strrep += ", ";
            strrep += std::to_string( m( i, 2 ) ); strrep += ", ";
            strrep += std::to_string( m( i, 3 ) );
            strrep += ( i != 3 ) ? " ]\n\r  " : " ]";
        }
        strrep += " ]";

        return strrep;
    }

    //--------------------------------------------------------------------------------------------//
    //                                   Matrix4x4 helpers                                        //
    //--------------------------------------------------------------------------------------------//

    CMat4 Perspective( float32 fov, float32 aspect, float32 zNear, float32 zFar )
    {
        CMat4 resmat;
        float32 _ht = std::tan( ( fov / 2 ) * ( engine::PI / 180 ) );
        resmat( 0, 0 ) = 1 / ( _ht * aspect )  ; resmat( 0, 1 ) =    0      ; resmat( 0, 2 ) =                   0                      ; resmat( 0, 3 ) =  0;
        resmat( 1, 0 ) =          0            ; resmat( 1, 1 ) = 1 / _ht   ; resmat( 1, 2 ) =                   0                      ; resmat( 1, 3 ) =  0;
        resmat( 2, 0 ) =          0            ; resmat( 2, 1 ) =    0      ; resmat( 2, 2 ) = -( zFar + zNear ) / ( zFar - zNear )     ; resmat( 2, 3 ) = -2 * ( zFar * zNear ) / ( zFar - zNear );
        resmat( 3, 0 ) =          0            ; resmat( 3, 1 ) =    0      ; resmat( 3, 2 ) =                  -1                      ; resmat( 3, 3 ) =  0;
        return resmat;
    }

    CMat4 Ortho( float32 width, float32 height, float32 zNear, float32 zFar )
    {
        CMat4 resmat;
        resmat( 0, 0 ) =  1 / ( width / 2 )   ; resmat( 0, 1 ) =       0            ; resmat( 0, 2 ) =         0             ; resmat( 0, 3 ) = 0;
        resmat( 1, 0 ) =        0             ; resmat( 1, 1 ) = 1 / ( height / 2 ) ; resmat( 1, 2 ) =         0             ; resmat( 1, 3 ) = 0;
        resmat( 2, 0 ) =        0             ; resmat( 2, 1 ) =       0            ; resmat( 2, 2 ) = -2 / ( zFar - zNear ) ; resmat( 2, 3 ) = -( zFar + zNear ) / ( zFar - zNear );
        resmat( 3, 0 ) =        0             ; resmat( 3, 1 ) =       0            ; resmat( 3, 2 ) =         0             ; resmat( 3, 3 ) = 1;
        return resmat;
    }

    CMat4 LookAt( const CVec3& position, const CVec3& target, const CVec3& worldUp )
    {
        /* axes vectors for the coordinate system represented by the resulting transform */
        CVec3 _front, _right, _up;

        _front = ( position - target ).normalized();
        // in case in the same direction of the world-up vector, just use a standard coordinate system
        if ( _front == worldUp )
        {
            _front = worldUp;
            _right = CVec3( worldUp.z(), worldUp.x(), worldUp.y() );
            _up    = CVec3( worldUp.y(), worldUp.z(), worldUp.x() );
        }
        else if ( ( _front + worldUp ) == CVec3( 0.0f, 0.0f, 0.0f ) )
        {
            _front = -worldUp;
            _right = CVec3( worldUp.z(), worldUp.x(), worldUp.y() );
            _up    = CVec3( worldUp.y(), worldUp.z(), worldUp.x() );
        }
        else
        {
            _right = tinymath::cross( worldUp, _front ).normalized();
            _up    = tinymath::cross( _front, _right ).normalized();
        }

        CMat4 resmat;
        resmat( 0, 0 ) = _right.x();
        resmat( 1, 0 ) = _up.x();
        resmat( 2, 0 ) = _front.x();
        resmat( 3, 0 ) = 0;

        resmat( 0, 1 ) = _right.y();
        resmat( 1, 1 ) = _up.y();
        resmat( 2, 1 ) = _front.y();
        resmat( 3, 1 ) = 0;

        resmat( 0, 2 ) = _right.z();
        resmat( 1, 2 ) = _up.z();
        resmat( 2, 2 ) = _front.z();
        resmat( 3, 2 ) = 0;

        resmat( 0, 3 ) = -_right.dot( position );
        resmat( 1, 3 ) = -_up.dot( position );
        resmat( 2, 3 ) = -_front.dot( position );
        resmat( 3, 3 ) = 1;
        return resmat;
    }

    CMat4 Scale( const CVec3& v )
    {
        CMat4 resmat;
        resmat( 0, 0 ) = v.x();
        resmat( 1, 1 ) = v.y();
        resmat( 2, 2 ) = v.z();
        return resmat;
    }

    CMat4 Rotation( const CMat3& m )
    {
        CMat4 resmat;
        for ( size_t i = 0; i < 3; i++ )
            for( size_t j = 0; j < 3; j++ )
                resmat( i, j ) = m( i, j );
        return resmat;
    }

    CMat4 Translation( const CVec3& v )
    {
        CMat4 resmat;
        resmat( 0, 3 ) = v.x();
        resmat( 1, 3 ) = v.y();
        resmat( 2, 3 ) = v.z();
        return resmat;
    }

    CMat4 RotationX( float32 angle )
    {
        CMat4 resmat;
        const auto cs = std::cos( angle );
        const auto sn = std::sin( angle );
        resmat( 1, 1 ) = cs; resmat( 1, 2 ) = -sn;
        resmat( 2, 1 ) = sn; resmat( 2, 2 ) = cs;
        return resmat;
    }

    CMat4 RotationY( float32 angle )
    {
        CMat4 resmat;
        const auto cs = std::cos( angle );
        const auto sn = std::sin( angle );
        resmat( 0, 0 ) = cs; resmat( 0, 2 ) = sn;
        resmat( 2, 0 ) = -sn; resmat( 2, 2 ) = cs;
        return resmat;
    }

    CMat4 RotationZ( float32 angle )
    {
        CMat4 resmat;
        const auto cs = std::cos( angle );
        const auto sn = std::sin( angle );
        resmat( 0, 0 ) = cs; resmat( 0, 1 ) = -sn;
        resmat( 1, 0 ) = sn; resmat( 1, 1 ) = cs;
        return resmat;
    }

    //--------------------------------------------------------------------------------------------//
    //                                   CInd3 implementation                                     //
    //--------------------------------------------------------------------------------------------//

    CInd3::CInd3()
    {
        buff[0] = 0;
        buff[1] = 0;
        buff[2] = 0;
    }

    CInd3::CInd3( int32 v1, int32 v2, int32 v3 )
    {
        buff[0] = v1;
        buff[1] = v2;
        buff[2] = v3;
    }

    std::string ToString( const CInd3& ind3 )
    {
        std::string strrep;
        strrep += "[ ";
        strrep += std::to_string( ind3.buff[0] );
        strrep += ", ";
        strrep += std::to_string( ind3.buff[1] );
        strrep += ", ";
        strrep += std::to_string( ind3.buff[2] );
        strrep += " ]";
        return strrep;
    }

    //--------------------------------------------------------------------------------------------//
    //                                   Geometric helpers                                        //
    //--------------------------------------------------------------------------------------------//

    std::string ToString( const CLine& line )
    {
        std::string strrep;
        strrep += "start   : " + engine::ToString( line.start ) + "\n\r";
        strrep += "end     : " + engine::ToString( line.end ) + "\n\r";
        return strrep;
    }

    void ComputeFrameAxes( const CVec3& axis1, CVec3& axis2, CVec3& axis3, const CVec3& worldUp )
    {
        auto _front = axis1.normalized();

        // in case in the same direction of the world-up vector, just use a standard 
        // coordinate system, otherwise compute normally using cross products
        if ( _front == worldUp )
        {
            _front = worldUp;
            axis2 = CVec3( worldUp.z(), worldUp.x(), worldUp.y() );
            axis3 = CVec3( worldUp.y(), worldUp.z(), worldUp.x() );
        }
        else if ( ( _front + worldUp ) == CVec3( 0.0f, 0.0f, 0.0f ) )
        {
            _front = -worldUp;
            axis2 = CVec3( worldUp.z(), worldUp.x(), worldUp.y() );
            axis3 = CVec3( worldUp.y(), worldUp.z(), worldUp.x() );
        }
        else
        {
            axis2 = tinymath::cross( worldUp, _front ).normalized();
            axis3 = tinymath::cross( _front, axis2 ).normalized();
        }
    }

    float32 SignedDistToPlane( const CVec3& point, const CPlane& plane )
    {
        return ( point - plane.position ).dot( plane.normal.normalized() );
    }

    float32 DistToPlane( const CVec3& point, const CPlane& plane )
    {
        return std::abs( SignedDistToPlane( point, plane ) );
    }

    CVec3 ProjInPlane( const CVec3& point, const CPlane& plane )
    {
        return point - DistToPlane( point, plane ) * plane.normal.normalized();
    }

    std::array< CVec3, 8 > ComputeBoxCorners( const CBoundingBox& bbox )
    {
        std::array< CVec3, 8 > _cornersInModel = { CVec3( -0.5f * bbox.size.x(), -0.5f * bbox.size.y(), -0.5f * bbox.size.z() ),
                                                   CVec3(  0.5f * bbox.size.x(), -0.5f * bbox.size.y(), -0.5f * bbox.size.z() ),
                                                   CVec3(  0.5f * bbox.size.x(),  0.5f * bbox.size.y(), -0.5f * bbox.size.z() ),
                                                   CVec3( -0.5f * bbox.size.x(),  0.5f * bbox.size.y(), -0.5f * bbox.size.z() ),
                                                   CVec3( -0.5f * bbox.size.x(), -0.5f * bbox.size.y(),  0.5f * bbox.size.z() ),
                                                   CVec3(  0.5f * bbox.size.x(), -0.5f * bbox.size.y(),  0.5f * bbox.size.z() ),
                                                   CVec3(  0.5f * bbox.size.x(),  0.5f * bbox.size.y(),  0.5f * bbox.size.z() ),
                                                   CVec3( -0.5f * bbox.size.x(),  0.5f * bbox.size.y(),  0.5f * bbox.size.z() ) };

        std::array< CVec3, 8 > _cornersInWorld;
        for ( size_t i = 0; i < _cornersInModel.size(); i++ )
            _cornersInWorld[i] = CVec3( bbox.worldTransform * CVec4( _cornersInModel[i], 1.0f ) );

        return _cornersInWorld;
    }

    CFrustum::CFrustum( const CMat4& viewProjMat )
    {
        CMat4 _invViewProjMat = tinymath::inverse( viewProjMat );

        std::array< CVec3, 8 > _frustumPointsClipSpace = {
            /*      near plane      */
            CVec3( -1.0f, -1.0f, -1.0f ), 
            CVec3( 1.0f, -1.0f, -1.0f ),
            CVec3( 1.0f,  1.0f, -1.0f ),
            CVec3( -1.0f,  1.0f, -1.0f ),
            /*      far plane       */
            CVec3( -1.0f, -1.0f, 1.0f ), 
            CVec3( 1.0f, -1.0f, 1.0f ),
            CVec3( 1.0f,  1.0f, 1.0f ),
            CVec3( -1.0f,  1.0f, 1.0f )
        };

        // compute corners of the frustum
        for ( size_t q = 0; q < corners.size(); q++ )
        {
            CVec4 _pointFrustum = _invViewProjMat * CVec4( _frustumPointsClipSpace[q], 1.0f );
            corners[q] = CVec3( _pointFrustum.x() / _pointFrustum.w(),
                                _pointFrustum.y() / _pointFrustum.w(),
                                _pointFrustum.z() / _pointFrustum.w() );
        }

        // compute planes of the frustum

        /* front-plane */
        planes[0].position = corners[0];
        planes[0].normal = tinymath::cross( corners[1] - corners[0], corners[3] - corners[0] ).normalized();

        /* back-plane */
        planes[1].position = corners[5];
        planes[1].normal = tinymath::cross( corners[4] - corners[5], corners[6] - corners[5] ).normalized();

        /* left-plane */
        planes[2].position = corners[4];
        planes[2].normal = tinymath::cross( corners[0] - corners[4], corners[7] - corners[4] ).normalized();

        /* right-plane */
        planes[3].position = corners[1];
        planes[3].normal = tinymath::cross( corners[5] - corners[1], corners[2] - corners[1] ).normalized();

        /* up-plane */
        planes[4].position = corners[2];
        planes[4].normal = tinymath::cross( corners[6] - corners[2], corners[3] - corners[2] ).normalized();

        /* down-plane */
        planes[5].position = corners[4];
        planes[5].normal = tinymath::cross( corners[5] - corners[4], corners[0] - corners[4] ).normalized();
    }

    bool CertainlyOutsideFrustum( const CFrustum& frustum, const CBoundingBox& bbox )
    {
        CComparatorSignedDistancePlane _comparator;
        auto _corners = ComputeBoxCorners( bbox );

        // Do the check with min-vertex only to discard most-outsiders. For cases in which ...
        // false is returned, the bbox might be inside, intersecting, or even outside in weird cases, ...
        // but for culling this does at least removes most of the true fully outsiders. Computing ...
        // a complete check if inside, outside or intersecting, seems to require quite a bit more of ...
        // computation, so for now we keep this discard method that removes most outsiders
        for ( size_t i = 0; i < frustum.planes.size(); i++ )
        {
            _comparator.plane = frustum.planes[i];
            auto _minVertex = *std::min_element( _corners.begin(), _corners.end(), _comparator );
            if ( SignedDistToPlane( _minVertex, frustum.planes[i] ) > 0.0f )
                return true;
        }

        return false;
    }

    bool CertainlyOutsideFrustum( const CFrustum& frustum, const CBoundingSphere& bsphere )
    {
        // Similarly to the previous case, discard most outsiders
        for ( size_t i = 0; i < frustum.planes.size(); i++ )
            if ( SignedDistToPlane( bsphere.worldPosition, frustum.planes[i] ) > bsphere.radius )
                return true;

        return false;
    }

    void ComputeMinMaxVertexToPlane( const CPlane& plane, const CBoundingBox& bbox, CVec3& minVertex, CVec3& maxVertex )
    {
        CComparatorSignedDistancePlane _comparator;
        _comparator.plane = plane;

        auto _corners = ComputeBoxCorners( bbox );
        std::sort( _corners.begin(), _corners.end(), _comparator );

        minVertex = _corners.front();
        maxVertex = _corners.back();
    }

    float32 ToRadians( float32 angle )
    {
        return angle * engine::PI / 180;
    }

    float32 ToDegrees( float32 angle )
    {
        return angle * 180 / engine::PI;
    }
}
