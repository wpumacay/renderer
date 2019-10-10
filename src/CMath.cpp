
#include <CMath.h>

namespace engine
{

    /**************************************************************************
    *                           CVec2 implementation                          *
    ***************************************************************************/

    CVec2::CVec2()
    {
        this->x = 0.0f;
        this->y = 0.0f;
    }

    CVec2::CVec2( float32 x, float32 y )
    {
        this->x = x;
        this->y = y;
    }

    void CVec2::normalize()
    {
        float32 _len = std::sqrt( x * x + y * y );
        x = x / _len;
        y = y / _len;
    }

    void CVec2::scale( float32 sx, float32 sy )
    {
        x *= sx;
        y *= sy;
    }

    void CVec2::scale( const CVec2& vscale )
    {
        x *= vscale.x;
        y *= vscale.y;
    }

    float32 CVec2::length()
    {
        return std::sqrt( x * x + y * y );
    }

    CVec2 CVec2::normalize( const CVec2& v )
    {
        CVec2 _res = v;

        _res.normalize();

        return _res;
    }

    CVec2 CVec2::scale( const CVec2& v, float32 val )
    {
        CVec2 _res = v;

        _res.scale( val, val );

        return _res;
    }

    CVec2 CVec2::scale( const CVec2& v, const CVec2& vals )
    {
        CVec2 _res = v;

        _res.scale( vals );

        return _res;
    }

    float32 CVec2::dot( const CVec2& v1, const CVec2& v2 )
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    bool CVec2::equal( const CVec2& v1, const CVec2& v2 )
    {
        CVec2 _delta = v1 - v2;

        float32 _l = _delta.length();

        if ( _l < ENGINE_EPS )
            return true;

        return false;
    }

    std::string toString( const CVec2& v )
    {
        std::string _strRep;

        _strRep += "( ";  _strRep += std::to_string( v.x );
        _strRep += " , "; _strRep += std::to_string( v.y );
        _strRep += " )";

        return _strRep;
    }

    CVec2 operator+ ( const CVec2& v1, const CVec2& v2 )
    {
        return { v1.x + v2.x, v1.y + v2.y };
    }

    CVec2 operator- ( const CVec2& v1, const CVec2& v2 )
    {
        return { v1.x - v2.x, v1.y - v2.y };
    }

    CVec2 operator* ( const CVec2& v, float32 s )
    {
        return { v.x * s, v.y * s };
    }

    CVec2 operator* ( float32 s, const CVec2& v )
    {
        return { v.x * s, v.y * s };
    }

    /**************************************************************************
    *                           CVec3 implementation                          *
    ***************************************************************************/

    CVec3::CVec3()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    CVec3::CVec3( float32 x, float32 y, float32 z )
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    CVec3::CVec3( const CVec4& v4 )
    {
        this->x = v4.x;
        this->y = v4.y;
        this->z = v4.z;
    }

    void CVec3::normalize()
    {
        float32 _len = std::sqrt( x * x + y * y + z * z );
        x = x / _len;
        y = y / _len;
        z = z / _len;
    }

    float32 CVec3::length( const CVec3& v )
    {
        return std::sqrt( v.x * v.x + v.y * v.y + v.z * v.z );
    }

    void CVec3::scale( float32 sx, float32 sy, float32 sz )
    {
        x *= sx;
        y *= sy;
        z *= sz;
    }

    void CVec3::scale( const CVec3& vscale )
    {
        x *= vscale.x;
        y *= vscale.y;
        z *= vscale.z;
    }

    float32 CVec3::length()
    {
        return std::sqrt( x * x + y * y + z * z );
    }

    CVec3 CVec3::normalize( const CVec3& v )
    {
        CVec3 _res = v;

        _res.normalize();

        return _res;
    }

    CVec3 CVec3::scale( const CVec3& v, float32 val )
    {
        CVec3 _res = v;

        _res.scale( val, val, val );

        return _res;
    }

    CVec3 CVec3::scale( const CVec3& v, const CVec3& vals )
    {
        CVec3 _res = v;

        _res.scale( vals );

        return _res;
    }

    CVec3 CVec3::cross( const CVec3& v1, const CVec3& v2 )
    {
        CVec3 _res;

        _res.x = v1.y * v2.z - v2.y * v1.z;
        _res.y = -v1.x * v2.z + v2.x * v1.z;
        _res.z = v1.x * v2.y - v2.x * v1.y;

        return _res;
    }

    float32 CVec3::dot( const CVec3& v1, const CVec3& v2 )
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    bool CVec3::equal( const CVec3& v1, const CVec3& v2 )
    {
        CVec3 _delta = v1 - v2;

        float32 _l = _delta.length();

        if ( _l < ENGINE_EPS )
            return true;

        return false;
    }

    std::string toString( const CVec3& v )
    {
        std::string _strRep;

        _strRep += "( ";  _strRep += std::to_string( v.x );
        _strRep += " , "; _strRep += std::to_string( v.y );
        _strRep += " , "; _strRep += std::to_string( v.z );
        _strRep += " )";

        return _strRep;
    }

    CVec3 operator+ ( const CVec3& v1, const CVec3& v2 )
    {
        return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
    }

    CVec3 operator- ( const CVec3& v1, const CVec3& v2 )
    {
        return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
    }

    CVec3 operator- ( const CVec3& v )
    {
        return { -v.x, -v.y, -v.z };
    }

    CVec3 operator* ( const CVec3& v, float32 s )
    {
        return { v.x * s, v.y * s, v.z * s };
    }

    CVec3 operator* ( float32 s, const CVec3& v )
    {
        return { v.x * s, v.y * s, v.z * s };
    }

    /**************************************************************************
    *                           CLine implementation                          *
    ***************************************************************************/

    CLine::CLine()
    {
        start = { 0.0f, 0.0f, 0.0f };
        end = { 0.0f, 0.0f, 0.0f };
    }

    CLine::CLine( const CVec3& start, const CVec3& end )
    {
        this->start = start;
        this->end = end;
    }

    std::string toString( const CLine& line )
    {
        std::string _strRep;

        _strRep += "start   : " + engine::toString( line.start ) + "\n\r";
        _strRep += "end     : " + engine::toString( line.end ) + "\n\r";

        return _strRep;
    }

    /**************************************************************************
    *                           CVec4 implementation                          *
    ***************************************************************************/

    CVec4::CVec4()
    {
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
        this->w = 0.0f;
    }

    CVec4::CVec4( float32 x, float32 y, float32 z, float32 w )
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    CVec4::CVec4( const CVec3& vec3, float32 w )
    {
        this->x = vec3.x;
        this->y = vec3.y;
        this->z = vec3.z;
        this->w = w;
    }

    std::string toString( const CVec4& vec4 )
    {
        std::string _strRep;

        _strRep += "( ";  _strRep += std::to_string( vec4.x );
        _strRep += " , "; _strRep += std::to_string( vec4.y );
        _strRep += " , "; _strRep += std::to_string( vec4.z );
        _strRep += " , "; _strRep += std::to_string( vec4.w );
        _strRep += " )";

        return _strRep;
    }

    /**************************************************************************
    *                           CMat4 implementation                          *
    ***************************************************************************/

    CMat4::CMat4()
    {
        buff[0] = 1; buff[4] = 0; buff[8]  = 0; buff[12] = 0;
        buff[1] = 0; buff[5] = 1; buff[9]  = 0; buff[13] = 0;
        buff[2] = 0; buff[6] = 0; buff[10] = 1; buff[14] = 0;
        buff[3] = 0; buff[7] = 0; buff[11] = 0; buff[15] = 1;
    }

    CMat4::CMat4( const CVec3& xAxis, const CVec3& yAxis, const CVec3& zAxis )
    {
        buff[0] = xAxis.x ; buff[4] = yAxis.x ; buff[8]  = zAxis.x ; buff[12] = 0;
        buff[1] = xAxis.y ; buff[5] = yAxis.y ; buff[9]  = zAxis.y ; buff[13] = 0;
        buff[2] = xAxis.z ; buff[6] = yAxis.z ; buff[10] = zAxis.z ; buff[14] = 0;
        buff[3] =    0    ; buff[7] =    0    ; buff[11] =    0    ; buff[15] = 1;
    }

    CMat4::CMat4( const CMat4& other )
    {
        for ( int i = 0; i < 16; i++ )
            buff[i] = other.buff[i];
    }

    void CMat4::set( int row, int col, float32 val )
    {
        buff[row + col * 4] = val;
    }

    float32 CMat4::get( int row, int col ) const
    {
        return buff[row + col * 4];
    }

    CVec3 CMat4::getBasisVectorX() const
    {
        return CVec3( buff[0], buff[1], buff[2] );
    }

    CVec3 CMat4::getBasisVectorY() const
    {
        return CVec3( buff[4], buff[5], buff[6] );
    }

    CVec3 CMat4::getBasisVectorZ() const
    {
        return CVec3( buff[8], buff[9], buff[10] );
    }

    CVec3 CMat4::getPosition() const
    {
        return CVec3( buff[12], buff[13], buff[14] );
    }

    CMat4 CMat4::getRotation() const
    {
        CMat4 _res;

        _res.buff[0] = buff[0]; _res.buff[4] = buff[4]; _res.buff[8] = buff[8];
        _res.buff[1] = buff[1]; _res.buff[5] = buff[5]; _res.buff[9] = buff[9];
        _res.buff[2] = buff[2]; _res.buff[6] = buff[6]; _res.buff[10] = buff[10];

        return _res;
    }

    CMat4 CMat4::inverse() const
    {
        CMat4 _res;

        float32 _r11 = get( 0, 0 ); float32 _r12 = get( 0, 1 );
        float32 _r21 = get( 1, 0 ); float32 _r22 = get( 1, 1 );
        float32 _r31 = get( 2, 0 ); float32 _r32 = get( 2, 1 );
        float32 _r41 = get( 3, 0 ); float32 _r42 = get( 3, 1 );

        float32 _r13 = get( 0, 2 ); float32 _r14 = get( 0, 3 );
        float32 _r23 = get( 1, 2 ); float32 _r24 = get( 1, 3 );
        float32 _r33 = get( 2, 2 ); float32 _r34 = get( 2, 3 );
        float32 _r43 = get( 3, 2 ); float32 _r44 = get( 3, 3 );

        // Calculate some intermediate values - "minors" of order 2
        float32 _m3434 = _r33 * _r44 - _r43 * _r34;
        float32 _m2434 = _r23 * _r44 - _r43 * _r24;
        float32 _m1434 = _r13 * _r44 - _r43 * _r14;
        float32 _m2334 = _r23 * _r34 - _r33 * _r24;
        float32 _m1334 = _r13 * _r34 - _r33 * _r14;
        float32 _m1234 = _r13 * _r24 - _r23 * _r14;

        float32 _m2312 = _r21 * _r32 - _r31 * _r22;
        float32 _m2412 = _r21 * _r42 - _r41 * _r22;
        float32 _m3412 = _r31 * _r42 - _r41 * _r32;
        float32 _m1312 = _r11 * _r32 - _r31 * _r12;
        float32 _m1412 = _r11 * _r42 - _r41 * _r12;
        float32 _m1212 = _r11 * _r22 - _r21 * _r12;        

        float32 _det = _r11 * ( _r22 * _m3434 - _r32 * _m2434 + _r42 * _m2334 ) -
                       _r21 * ( _r12 * _m3434 - _r32 * _m1434 + _r42 * _m1334 ) +
                       _r31 * ( _r12 * _m2434 - _r22 * _m1434 + _r42 * _m1234 ) - 
                       _r41 * ( _r12 * _m2334 - _r22 * _m1334 + _r32 * _m1234 );

        float32 _invdet = 1 / _det;

        // Generate transpose of "cofactors" matrix ( also divide by determinant ) in place
        _res.set( 0, 0, ( _r22 * _m3434 - _r32 * _m2434 + _r42 * _m2334 ) *  _invdet );
        _res.set( 0, 1, ( _r12 * _m3434 - _r32 * _m1434 + _r42 * _m1334 ) * -_invdet );
        _res.set( 0, 2, ( _r12 * _m2434 - _r22 * _m1434 + _r42 * _m1234 ) *  _invdet );
        _res.set( 0, 3, ( _r12 * _m2334 - _r22 * _m1334 + _r32 * _m1234 ) * -_invdet );

        _res.set( 1, 0, ( _r21 * _m3434 - _r31 * _m2434 + _r41 * _m2334 ) * -_invdet );
        _res.set( 1, 1, ( _r11 * _m3434 - _r31 * _m1434 + _r41 * _m1334 ) *  _invdet );
        _res.set( 1, 2, ( _r11 * _m2434 - _r21 * _m1434 + _r41 * _m1234 ) * -_invdet );
        _res.set( 1, 3, ( _r11 * _m2334 - _r21 * _m1334 + _r31 * _m1234 ) *  _invdet );

        _res.set( 2, 0, ( _r44 * _m2312 - _r34 * _m2412 + _r24 * _m3412 ) *  _invdet );
        _res.set( 2, 1, ( _r44 * _m1312 - _r34 * _m1412 + _r14 * _m3412 ) * -_invdet );
        _res.set( 2, 2, ( _r44 * _m1212 - _r24 * _m1412 + _r14 * _m2412 ) *  _invdet );
        _res.set( 2, 3, ( _r34 * _m1212 - _r24 * _m1312 + _r14 * _m2312 ) * -_invdet );

        _res.set( 3, 0, ( _r43 * _m2312 - _r33 * _m2412 + _r23 * _m3412 ) * -_invdet );
        _res.set( 3, 1, ( _r43 * _m1312 - _r33 * _m1412 + _r13 * _m3412 ) *  _invdet );
        _res.set( 3, 2, ( _r43 * _m1212 - _r23 * _m1412 + _r13 * _m2412 ) * -_invdet );
        _res.set( 3, 3, ( _r33 * _m1212 - _r23 * _m1312 + _r13 * _m2312 ) *  _invdet );

        return _res;
    }

    CMat4 CMat4::transpose() const
    {
        CMat4 _res;

        for ( size_t i = 0; i < 4; i++ )
            for ( size_t j = 0; j < 4; j++ )
                _res.set( i, j, get( j, i ) );

        return _res;
    }

    CMat4 CMat4::perspective( float32 fov, float32 aspect, float32 zNear, float32 zFar )
    {
        CMat4 _res;

        float32 _ht = tan( ( fov / 2.0f ) * ( ENGINE_PI / 180.0f ) );

        _res.buff[0] = 1 / ( _ht * aspect )  ; _res.buff[4] =    0      ; _res.buff[8]  =                   0                      ; _res.buff[12] =  0;
        _res.buff[1] =          0            ; _res.buff[5] = 1 / _ht   ; _res.buff[9]  =                   0                      ; _res.buff[13] =  0;
        _res.buff[2] =          0            ; _res.buff[6] =    0      ; _res.buff[10] = -( zFar + zNear ) / ( zFar - zNear )     ; _res.buff[14] = -2.0f * ( zFar * zNear ) / ( zFar - zNear );
        _res.buff[3] =          0            ; _res.buff[7] =    0      ; _res.buff[11] =                  -1                      ; _res.buff[15] =  0;

        return _res;
    }

    CMat4 CMat4::ortho( float32 width, float32 height, float32 zNear, float32 zFar )
    {
        CMat4 _res;

        _res.buff[0] =  1 / ( width / 2 )   ; _res.buff[4] =       0            ; _res.buff[8]  =         0             ; _res.buff[12] = 0;
        _res.buff[1] =        0             ; _res.buff[5] = 1 / ( height / 2 ) ; _res.buff[9]  =         0             ; _res.buff[13] = 0;
        _res.buff[2] =        0             ; _res.buff[6] =       0            ; _res.buff[10] = -2 / ( zFar - zNear ) ; _res.buff[14] = -( zFar + zNear ) / ( zFar - zNear );
        _res.buff[3] =        0             ; _res.buff[7] =       0            ; _res.buff[11] =         0             ; _res.buff[15] = 1;

        return _res;
    }

    CMat4 CMat4::lookAt( const CVec3& position, const CVec3& target, const CVec3& worldUp )
    {
        /* axes vectors for the coordinate system represented by the resulting transform */
        CVec3 _front, _right, _up;

        _front = CVec3::normalize( position - target );
        // in case in the same direction of the world-up vector, just use a standard coordinate system
        if ( CVec3::equal( _front, worldUp ) )
        {
            _front = worldUp;
            _right = { worldUp.z, worldUp.x, worldUp.y };
            _up    = { worldUp.y, worldUp.z, worldUp.x };
        }
        else if ( CVec3::equal( _front + worldUp, { 0.0f, 0.0f, 0.0f } ) )
        {
            _front = -worldUp;
            _right = { worldUp.z, worldUp.x, worldUp.y };
            _up    = { worldUp.y, worldUp.z, worldUp.x };
        }
        else
        {
            _right = CVec3::normalize( CVec3::cross( worldUp, _front ) );
            _up    = CVec3::normalize( CVec3::cross( _front, _right ) );
        }

        CMat4 _res;

        _res.buff[0] = _right.x;
        _res.buff[1] = _up.x;
        _res.buff[2] = _front.x;
        _res.buff[3] = 0;

        _res.buff[4] = _right.y;
        _res.buff[5] = _up.y;
        _res.buff[6] = _front.y;
        _res.buff[7] = 0;

        _res.buff[8]  = _right.z;
        _res.buff[9]  = _up.z;
        _res.buff[10] = _front.z;
        _res.buff[11] = 0;

        _res.buff[12] = -CVec3::dot( _right, position );
        _res.buff[13] = -CVec3::dot( _up, position );
        _res.buff[14] = -CVec3::dot( _front, position );
        _res.buff[15] = 1;

        return _res;
    }

    CMat4 CMat4::scale( const CVec3& v )
    {
        CMat4 _res;

        _res.buff[0]  = v.x;
        _res.buff[5]  = v.y;
        _res.buff[10] = v.z;

        return _res;
    }

    CMat4 CMat4::translation( const CVec3& v )
    {
        CMat4 _res;

        _res.buff[12] = v.x;
        _res.buff[13] = v.y;
        _res.buff[14] = v.z;

        return _res;
    }

    CMat4 CMat4::rotationX( float32 theta )
    {
        CMat4 _res;

        _res.buff[5] = cos( theta );
        _res.buff[6] = sin( theta );
        _res.buff[9] = -sin( theta );
        _res.buff[10] = cos( theta );

        return _res;
    }

    CMat4 CMat4::rotationY( float32 theta )
    {
        CMat4 _res;

        _res.buff[0] = cos( theta );
        _res.buff[2] = -sin( theta );
        _res.buff[8] = sin( theta );
        _res.buff[10] = cos( theta );

        return _res;
    }

    CMat4 CMat4::rotationZ( float32 theta )
    {
        CMat4 _res;

        _res.buff[0] = cos( theta );
        _res.buff[1] = sin( theta );
        _res.buff[4] = -sin( theta );
        _res.buff[5] = cos( theta );

        return _res;
    }

    CMat4 CMat4::rotation( float32 theta, const eAxis& axis )
    {
        if ( axis == eAxis::X ) return rotationX( theta );
        if ( axis == eAxis::Y ) return rotationY( theta );
        if ( axis == eAxis::Z ) return rotationZ( theta );

        return CMat4();
    }

    CMat4 CMat4::rotation( float32 theta, const CVec3& axis )
    {
        CMat4 _res;

        auto _nAxis = CVec3::normalize( axis );
        float32 _c = std::cos( theta );
        float32 _s = std::sin( theta );
        float32 _ccomp = 1.0f - _c;
        float32 _x = _nAxis.x;
        float32 _y = _nAxis.y;
        float32 _z = _nAxis.z;

        _res.buff[0] = _x * _x * _ccomp + _c;      _res.buff[4] = _x * _y * _ccomp - _z * _s; _res.buff[8] = _x * _z * _ccomp + _y * _s;
        _res.buff[1] = _y * _x * _ccomp + _z * _s; _res.buff[5] = _y * _y * _ccomp + _c;      _res.buff[9] = _y * _z * _ccomp - _x * _s;
        _res.buff[2] = _z * _x * _ccomp - _y * _s; _res.buff[6] = _z * _y * _ccomp + _x * _s; _res.buff[10] = _z * _z * _ccomp + _c;

        return _res;
    }

    CMat4 CMat4::fromEuler( const CVec3& euler )
    {
        CMat4 _res;

        float _cz = std::cos( euler.z );
        float _sz = std::sin( euler.z );
        float _cy = std::cos( euler.y );
        float _sy = std::sin( euler.y );
        float _cx = std::cos( euler.x );
        float _sx = std::sin( euler.x );

        _res.buff[0] = _cz * _cy;
        _res.buff[1] = _sz * _cy;
        _res.buff[2] = -_sy;

        _res.buff[4] = _cz * _sy * _sx - _sz * _cx;
        _res.buff[5] = _cz * _cx + _sz * _sy * _sx;
        _res.buff[6] = _cy * _sx;

        _res.buff[8] = _sz * _sx + _cz * _sy * _cx;
        _res.buff[9] = _sz * _sy * _cx - _cz * _sx;
        _res.buff[10] = _cy * _cx;

        return _res;
    }

    CMat4 CMat4::fromEulerIntrinsicXYZ( const CVec3& euler )
    {
        CMat4 _res;

        float _cz = std::cos( euler.z );
        float _sz = std::sin( euler.z );
        float _cy = std::cos( euler.y );
        float _sy = std::sin( euler.y );
        float _cx = std::cos( euler.x );
        float _sx = std::sin( euler.x );

        _res.buff[0] = _cy * _cz;
        _res.buff[1] = _cx * _sz + _sx * _sy * _cz;
        _res.buff[2] = _sx * _sz - _cx * _sy * _cz;

        _res.buff[4] = -_cy * _sz;
        _res.buff[5] = _cx * _cz - _sx * _sy * _sz;
        _res.buff[6] = _sx * _cz + _cx * _sy * _sz;

        _res.buff[8] = _sy;
        _res.buff[9] = -_sx * _cy;
        _res.buff[10] = _cx * _cy;

        return _res;
    }

    std::string toString( const CMat4& mat )
    {
        std::string _res;

        _res += "[ ";
        for ( int i = 0; i < 4; i++ )
        {
            _res += "[ ";
            _res += std::to_string( mat.get( i, 0 ) ); _res += ", ";
            _res += std::to_string( mat.get( i, 1 ) ); _res += ", ";
            _res += std::to_string( mat.get( i, 2 ) ); _res += ", ";
            _res += std::to_string( mat.get( i, 3 ) );
            _res += ( i != 3 ) ? " ]\n\r  " : " ]";
        }
        _res += " ]";

        return _res;
    }

    CMat4 operator* ( const CMat4& mat1, const CMat4& mat2 )
    {
        CMat4 _res;

        for ( int i = 0; i < 4; i++ )
        {
            for ( int j = 0; j < 4; j++ )
            {
                _res.set( i, j, 0.0f );

                for ( int k = 0; k < 4; k++ )
                {
                    // Matrices are stored in column major form, so ...
                    // we use this indexing for the multiplication
                    // k + 4 * (fixed) -> over column
                    // (fixed) + 4 * k -> over row
                    _res.set( i, j, _res.get( i, j ) + mat1.get( i, k ) * mat2.get( k, j ) );
                }
            }
        }

        return _res;
    }

    CVec4 operator* ( const CMat4& mat, const CVec4& vec )
    {
        CVec4 _res;
        _res.x = mat.get( 0, 0 ) * vec.x + mat.get( 0, 1 ) * vec.y + 
                 mat.get( 0, 2 ) * vec.z + mat.get( 0, 3 ) * vec.w;

        _res.y = mat.get( 1, 0 ) * vec.x + mat.get( 1, 1 ) * vec.y + 
                 mat.get( 1, 2 ) * vec.z + mat.get( 1, 3 ) * vec.w;

        _res.z = mat.get( 2, 0 ) * vec.x + mat.get( 2, 1 ) * vec.y + 
                 mat.get( 2, 2 ) * vec.z + mat.get( 2, 3 ) * vec.w;

        _res.w = mat.get( 3, 0 ) * vec.x + mat.get( 3, 1 ) * vec.y + 
                 mat.get( 3, 2 ) * vec.z + mat.get( 3, 3 ) * vec.w;

        return _res;
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
        std::string _strRep;

        _strRep += "[ ";
        _strRep += std::to_string( ind3.buff[0] );
        _strRep += ", ";
        _strRep += std::to_string( ind3.buff[1] );
        _strRep += ", ";
        _strRep += std::to_string( ind3.buff[2] );
        _strRep += " ]";

        return _strRep;
    }

    /**************************************************************************
    *                           Geometric helpers                             *
    ***************************************************************************/

    void computeFrameAxes( const CVec3& axis1, CVec3& axis2, CVec3& axis3, const CVec3& worldUp )
    {
        auto _front = CVec3::normalize( axis1 );

        // in case in the same direction of the world-up vector, just use a standard 
        // coordinate system, otherwise compute normally using cross products
        if ( CVec3::equal( _front, worldUp ) )
        {
            _front = worldUp;
            axis2 = { worldUp.z, worldUp.x, worldUp.y };
            axis3 = { worldUp.y, worldUp.z, worldUp.x };
        }
        else if ( CVec3::equal( _front + worldUp, { 0.0f, 0.0f, 0.0f } ) )
        {
            _front = -worldUp;
            axis2 = { worldUp.z, worldUp.x, worldUp.y };
            axis3 = { worldUp.y, worldUp.z, worldUp.x };
        }
        else
        {
            axis2 = CVec3::normalize( CVec3::cross( worldUp, _front ) );
            axis3 = CVec3::normalize( CVec3::cross( _front, axis2 ) );
        }
    }

    float32 signedDistToPlane( const CVec3& point, const CPlane& plane )
    {
        return CVec3::dot( point - plane.position, CVec3::normalize( plane.normal ) );
    }

    float32 distToPlane( const CVec3& point, const CPlane& plane )
    {
        return std::abs( signedDistToPlane( point, plane ) );
    }

    CVec3 projInPlane( const CVec3& point, const CPlane& plane )
    {
        return point - distToPlane( point, plane ) * CVec3::normalize( plane.normal );
    }

    std::array< CVec3, 8 > computeBoxCorners( const CBoundingBox& bbox )
    {
        std::array< CVec3, 8 > _cornersInModel = { CVec3( -0.5f * bbox.size.x, -0.5f * bbox.size.y, -0.5f * bbox.size.z ),
                                                   CVec3(  0.5f * bbox.size.x, -0.5f * bbox.size.y, -0.5f * bbox.size.z ),
                                                   CVec3(  0.5f * bbox.size.x,  0.5f * bbox.size.y, -0.5f * bbox.size.z ),
                                                   CVec3( -0.5f * bbox.size.x,  0.5f * bbox.size.y, -0.5f * bbox.size.z ),
                                                   CVec3( -0.5f * bbox.size.x, -0.5f * bbox.size.y,  0.5f * bbox.size.z ),
                                                   CVec3(  0.5f * bbox.size.x, -0.5f * bbox.size.y,  0.5f * bbox.size.z ),
                                                   CVec3(  0.5f * bbox.size.x,  0.5f * bbox.size.y,  0.5f * bbox.size.z ),
                                                   CVec3( -0.5f * bbox.size.x,  0.5f * bbox.size.y,  0.5f * bbox.size.z ) };

        std::array< CVec3, 8 > _cornersInWorld;
        for ( size_t i = 0; i < _cornersInModel.size(); i++ )
            _cornersInWorld[i] = CVec3( bbox.worldTransform * CVec4( _cornersInModel[i], 1.0f ) );

        return _cornersInWorld;
    }

    CFrustum::CFrustum( const CMat4& viewProjMat )
    {
        CMat4 _invViewProjMat = viewProjMat.inverse();

        CVec3 _frustumPointsClipSpace[8] = {
            /*      near plane      */
            { -1.0f, -1.0f, -1.0f }, 
            { 1.0f, -1.0f, -1.0f },
            { 1.0f,  1.0f, -1.0f },
            { -1.0f,  1.0f, -1.0f },
            /*      far plane       */
            { -1.0f, -1.0f, 1.0f }, 
            { 1.0f, -1.0f, 1.0f },
            { 1.0f,  1.0f, 1.0f },
            { -1.0f,  1.0f, 1.0f }
        };

        // compute corners of the frustum
        for ( size_t q = 0; q < corners.size(); q++ )
        {
            CVec4 _pointFrustum = _invViewProjMat * CVec4( _frustumPointsClipSpace[q], 1.0f );
            corners[q] = CVec3( _pointFrustum.x / _pointFrustum.w,
                                _pointFrustum.y / _pointFrustum.w,
                                _pointFrustum.z / _pointFrustum.w );
        }

        // compute planes of the frustum

        /* front-plane */
        planes[0].position = corners[0];
        planes[0].normal = CVec3::normalize( CVec3::cross( corners[1] - corners[0], corners[3] - corners[0] ) );

        /* back-plane */
        planes[1].position = corners[5];
        planes[1].normal = CVec3::normalize( CVec3::cross( corners[4] - corners[5], corners[6] - corners[5] ) );

        /* left-plane */
        planes[2].position = corners[4];
        planes[2].normal = CVec3::normalize( CVec3::cross( corners[0] - corners[4], corners[7] - corners[4] ) );

        /* right-plane */
        planes[3].position = corners[1];
        planes[3].normal = CVec3::normalize( CVec3::cross( corners[5] - corners[1], corners[2] - corners[1] ) );

        /* up-plane */
        planes[4].position = corners[2];
        planes[4].normal = CVec3::normalize( CVec3::cross( corners[6] - corners[2], corners[3] - corners[2] ) );

        /* down-plane */
        planes[5].position = corners[4];
        planes[5].normal = CVec3::normalize( CVec3::cross( corners[5] - corners[4], corners[0] - corners[4] ) );
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
        return angle * ENGINE_PI / 180.0f;
    }

    float32 toDegrees( float32 angle )
    {
        return angle * 180.0f / ENGINE_PI;
    }
}