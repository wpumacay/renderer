// @todo: move all helpers to tiny_math (should be templated)

#include <CMath.h>

namespace engine
{

    /**************************************************************************
    *                          Vec and Mat helpers                            *
    ***************************************************************************/

    bool operator==( const CVec3& v1, const CVec3& v2 )
    {
        return ( v1 - v2 ).length() < ENGINE_EPS;
    }

    bool operator!=( const CVec3& v1, const CVec3& v2 )
    {
        return !(v1 == v2);
    }

    CVec3 operator- ( const CVec3& v )
    {
        return CVec3( -v.x(), -v.y(), -v.z() );
    }

    std::string toString( const CVec2& v )
    {
        std::string _strrep;

        _strrep += "( ";  _strrep += std::to_string( v.x() );
        _strrep += " , "; _strrep += std::to_string( v.y() );
        _strrep += " )";

        return _strrep;
    }

    std::string toString( const CVec3& v )
    {
        std::string _strrep;

        _strrep += "( ";  _strrep += std::to_string( v.x() );
        _strrep += " , "; _strrep += std::to_string( v.y() );
        _strrep += " , "; _strrep += std::to_string( v.z() );
        _strrep += " )";

        return _strrep;
    }

    std::string toString( const CVec4& v )
    {
        std::string _strrep;

        _strrep += "( ";  _strrep += std::to_string( v.x() );
        _strrep += " , "; _strrep += std::to_string( v.y() );
        _strrep += " , "; _strrep += std::to_string( v.z() );
        _strrep += " , "; _strrep += std::to_string( v.w() );
        _strrep += " )";

        return _strrep;
    }

    std::string toString( const CMat2& m )
    {
        std::string _strrep;

        _strrep += "[ ";
        for ( int i = 0; i < 2; i++ )
        {
            _strrep += "[ ";
            _strrep += std::to_string( m( i, 0 ) ); _strrep += ", ";
            _strrep += std::to_string( m( i, 1 ) );
            _strrep += ( i != 1 ) ? " ]\n\r  " : " ]";
        }
        _strrep += " ]";

        return _strrep;
    }

    std::string toString( const CMat3& m )
    {
        std::string _strrep;

        _strrep += "[ ";
        for ( int i = 0; i < 3; i++ )
        {
            _strrep += "[ ";
            _strrep += std::to_string( m( i, 0 ) ); _strrep += ", ";
            _strrep += std::to_string( m( i, 1 ) ); _strrep += ", ";
            _strrep += std::to_string( m( i, 2 ) );
            _strrep += ( i != 2 ) ? " ]\n\r  " : " ]";
        }
        _strrep += " ]";

        return _strrep;
    }

    std::string toString( const CMat4& m )
    {
        std::string _strrep;

        _strrep += "[ ";
        for ( int i = 0; i < 4; i++ )
        {
            _strrep += "[ ";
            _strrep += std::to_string( m( i, 0 ) ); _strrep += ", ";
            _strrep += std::to_string( m( i, 1 ) ); _strrep += ", ";
            _strrep += std::to_string( m( i, 2 ) ); _strrep += ", ";
            _strrep += std::to_string( m( i, 3 ) );
            _strrep += ( i != 3 ) ? " ]\n\r  " : " ]";
        }
        _strrep += " ]";

        return _strrep;
    }


    /**************************************************************************
    *                           CLine implementation                          *
    ***************************************************************************/

    CLine::CLine()
    {
        start = CVec3( 0.0f, 0.0f, 0.0f );
        end = CVec3( 0.0f, 0.0f, 0.0f );
    }

    CLine::CLine( const CVec3& start, const CVec3& end )
    {
        this->start = start;
        this->end = end;
    }

    std::string toString( const CLine& line )
    {
        std::string _strrep;

        _strrep += "start   : " + engine::toString( line.start ) + "\n\r";
        _strrep += "end     : " + engine::toString( line.end ) + "\n\r";

        return _strrep;
    }

    /**************************************************************************
    *                            Matrix4x4 helpers                            *
    ***************************************************************************/

    CMat4 perspective( float32 fov, float32 aspect, float32 zNear, float32 zFar )
    {
        CMat4 _resmat;

        float32 _ht = std::tan( ( fov / 2 ) * ( ENGINE_PI / 180 ) );

        _resmat( 0, 0 ) = 1 / ( _ht * aspect )  ; _resmat( 0, 1 ) =    0      ; _resmat( 0, 2 ) =                   0                      ; _resmat( 0, 3 ) =  0;
        _resmat( 1, 0 ) =          0            ; _resmat( 1, 1 ) = 1 / _ht   ; _resmat( 1, 2 ) =                   0                      ; _resmat( 1, 3 ) =  0;
        _resmat( 2, 0 ) =          0            ; _resmat( 2, 1 ) =    0      ; _resmat( 2, 2 ) = -( zFar + zNear ) / ( zFar - zNear )     ; _resmat( 2, 3 ) = -2 * ( zFar * zNear ) / ( zFar - zNear );
        _resmat( 3, 0 ) =          0            ; _resmat( 3, 1 ) =    0      ; _resmat( 3, 2 ) =                  -1                      ; _resmat( 3, 3 ) =  0;

        return _resmat;
    }

    CMat4 ortho( float32 width, float32 height, float32 zNear, float32 zFar )
    {
        CMat4 _resmat;

        _resmat( 0, 0 ) =  1 / ( width / 2 )   ; _resmat( 0, 1 ) =       0            ; _resmat( 0, 2 ) =         0             ; _resmat( 0, 3 ) = 0;
        _resmat( 1, 0 ) =        0             ; _resmat( 1, 1 ) = 1 / ( height / 2 ) ; _resmat( 1, 2 ) =         0             ; _resmat( 1, 3 ) = 0;
        _resmat( 2, 0 ) =        0             ; _resmat( 2, 1 ) =       0            ; _resmat( 2, 2 ) = -2 / ( zFar - zNear ) ; _resmat( 2, 3 ) = -( zFar + zNear ) / ( zFar - zNear );
        _resmat( 3, 0 ) =        0             ; _resmat( 3, 1 ) =       0            ; _resmat( 3, 2 ) =         0             ; _resmat( 3, 3 ) = 1;

        return _resmat;
    }

    CMat4 lookAt( const CVec3& position, const CVec3& target, const CVec3& worldUp )
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

        CMat4 _resmat;

        _resmat( 0, 0 ) = _right.x();
        _resmat( 1, 0 ) = _up.x();
        _resmat( 2, 0 ) = _front.x();
        _resmat( 3, 0 ) = 0;

        _resmat( 0, 1 ) = _right.y();
        _resmat( 1, 1 ) = _up.y();
        _resmat( 2, 1 ) = _front.y();
        _resmat( 3, 1 ) = 0;

        _resmat( 0, 2 ) = _right.z();
        _resmat( 1, 2 ) = _up.z();
        _resmat( 2, 2 ) = _front.z();
        _resmat( 3, 2 ) = 0;

        _resmat( 0, 3 ) = -_right.dot( position );
        _resmat( 1, 3 ) = -_up.dot( position );
        _resmat( 2, 3 ) = -_front.dot( position );
        _resmat( 3, 3 ) = 1;

        return _resmat;
    }

    CMat4 scale( const CVec3& v )
    {
        CMat4 _resmat;

        _resmat( 0, 0 ) = v.x();
        _resmat( 1, 1 ) = v.y();
        _resmat( 2, 2 ) = v.z();

        return _resmat;
    }

    CMat4 rotation( const CMat3& m )
    {
        CMat4 _resmat;

        for ( size_t i = 0; i < 3; i++ )
            for( size_t j = 0; j < 3; j++ )
                _resmat( i, j ) = m( i, j );

        return _resmat;
    }

    CMat4 translation( const CVec3& v )
    {
        CMat4 _resmat;

        _resmat( 0, 3 ) = v.x();
        _resmat( 1, 3 ) = v.y();
        _resmat( 2, 3 ) = v.z();

        return _resmat;
    }

    /**************************************************************************
    *                           CInd3 implementation                          *
    ***************************************************************************/

    CInd3::CInd3()
    {
        buff[0] = 0;
        buff[1] = 0;
        buff[2] = 0;
    }

    CInd3::CInd3( GLint v1, GLint v2, GLint v3 )
    {
        buff[0] = v1;
        buff[1] = v2;
        buff[2] = v3;
    }

    std::string toString( const CInd3& ind3 )
    {
        std::string _strrep;

        _strrep += "[ ";
        _strrep += std::to_string( ind3.buff[0] );
        _strrep += ", ";
        _strrep += std::to_string( ind3.buff[1] );
        _strrep += ", ";
        _strrep += std::to_string( ind3.buff[2] );
        _strrep += " ]";

        return _strrep;
    }

    /**************************************************************************
    *                           Geometric helpers                             *
    ***************************************************************************/

    void computeFrameAxes( const CVec3& axis1, CVec3& axis2, CVec3& axis3, const CVec3& worldUp )
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

    float32 signedDistToPlane( const CVec3& point, const CPlane& plane )
    {
        return ( point - plane.position ).dot( plane.normal.normalized() );
    }

    float32 distToPlane( const CVec3& point, const CPlane& plane )
    {
        return std::abs( signedDistToPlane( point, plane ) );
    }

    CVec3 projInPlane( const CVec3& point, const CPlane& plane )
    {
        return point - distToPlane( point, plane ) * plane.normal.normalized();
    }

    std::array< CVec3, 8 > computeBoxCorners( const CBoundingBox& bbox )
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

    bool certainlyOutsideFrustum( const CFrustum& frustum, const CBoundingBox& bbox )
    {
        CComparatorSignedDistancePlane _comparator;
        auto _corners = computeBoxCorners( bbox );

        // Do the check with min-vertex only to discard most-outsiders. For cases in which ...
        // false is returned, the bbox might be inside, intersecting, or even outside in weird cases, ...
        // but for culling this does at least removes most of the true fully outsiders. Computing ...
        // a complete check if inside, outside or intersecting, seems to require quite a bit more of ...
        // computation, so for now we keep this discard method that removes most outsiders
        for ( size_t i = 0; i < frustum.planes.size(); i++ )
        {
            _comparator.plane = frustum.planes[i];
            auto _minVertex = *std::min_element( _corners.begin(), _corners.end(), _comparator );
            if ( signedDistToPlane( _minVertex, frustum.planes[i] ) > 0.0f )
                return true;
        }

        return false;
    }

    bool certainlyOutsideFrustum( const CFrustum& frustum, const CBoundingSphere& bsphere )
    {
        // Similarly to the previous case, discard most outsiders
        for ( size_t i = 0; i < frustum.planes.size(); i++ )
            if ( signedDistToPlane( bsphere.worldPosition, frustum.planes[i] ) > bsphere.radius )
                return true;

        return false;
    }

    void computeMinMaxVertexToPlane( const CPlane& plane, const CBoundingBox& bbox, CVec3& minVertex, CVec3& maxVertex )
    {
        CComparatorSignedDistancePlane _comparator;
        _comparator.plane = plane;

        auto _corners = computeBoxCorners( bbox );
        std::sort( _corners.begin(), _corners.end(), _comparator );

        minVertex = _corners.front();
        maxVertex = _corners.back();
    }

    /**************************************************************************
    *                             Extra helpers                               *
    ***************************************************************************/

    float32 toRadians( float32 angle )
    {
        return angle * ENGINE_PI / 180;
    }

    float32 toDegrees( float32 angle )
    {
        return angle * 180 / ENGINE_PI;
    }
}