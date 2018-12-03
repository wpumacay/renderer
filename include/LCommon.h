
#pragma once

#include <GL/glew.h>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

#define _PI 3.141592653589793

#define GL_TO_WORLD 1.0f

#define RENDERABLE_TYPE_BASE 0
#define RENDERABLE_TYPE_MESH 1
#define RENDERABLE_TYPE_MODEL 2
#define RENDERABLE_MAX_TYPE 3

#define MAX_DELTA 0.05f

namespace engine
{

    typedef unsigned char u8;

    struct LVec3
    {
        GLfloat x;
        GLfloat y;
        GLfloat z;

        LVec3()
        {
            this->x = 0;
            this->y = 0;
            this->z = 0;        
        }

        LVec3( GLfloat x, GLfloat y, GLfloat z )
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }
        
        LVec3 operator+( const LVec3& other )
        {
            return LVec3( this->x + other.x,
                          this->y + other.y,
                          this->z + other.z );
        }

        LVec3 operator-( const LVec3& other )
        {
            return LVec3( this->x - other.x,
                          this->y - other.y,
                          this->z - other.z );
        }

        void normalize()
        {
            GLfloat _len = sqrt( x * x + y * y + z * z );
            x = x / _len;
            y = y / _len;
            z = z / _len;
        }

        LVec3 operator*( float s )
        {
            return LVec3( this->x * s,
                          this->y * s,
                          this->z * s );
        }

        void scale( float sx, float sy, float sz )
        {
            x *= sx; y *= sy; z *= sz;
        }

        GLfloat length()
        {
            return sqrt( x * x + y * y + z * z );
        }

        string toString()
        {
            string _v;
            _v += "( ";  _v += to_string( x );
            _v += " , "; _v += to_string( y );
            _v += " , "; _v += to_string( z );
            _v += " )";

            return _v;
        }

        static LVec3 normalize( const LVec3& v )
        {
            LVec3 _res = v;

            _res.normalize();

            return _res;
        }

        static LVec3 cross( const LVec3& v1,
                            const LVec3& v2 )
        {
            LVec3 _res;

            _res.x = v1.y * v2.z - v2.y * v1.z;
            _res.y = -v1.x * v2.z + v2.x * v1.z;
            _res.z = v1.x * v2.y - v2.x * v1.y;

            return _res;
        }

        static float dot( const LVec3& v1,
                          const LVec3& v2 )
        {
            return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
        }

        static LVec3 plus( const LVec3& v1, const LVec3& v2 )
        {
            return LVec3( v1.x + v2.x, v1.y + v2.y, v1.z + v2.z );
        }

        static LVec3 minus( const LVec3& v1, const LVec3& v2 )
        {
            return LVec3( v1.x - v2.x, v1.y - v2.y, v1.z - v2.z );
        }

        static bool equal( LVec3 v1, LVec3 v2 )
        {
            LVec3 _delta = v1 - v2;

            GLfloat _l = _delta.length();

            if ( _l < 0.001 )
            {
                return true;
            }

            return false;
        }

    };

    LVec3 operator+ ( const LVec3& v1, const LVec3& v2 );
    LVec3 operator- ( const LVec3& v1, const LVec3& v2 );

    struct LLine
    {
        LVec3 start;
        LVec3 end;
    };

    struct LMat4
    {

        GLfloat buff[16];

        LMat4()
        {
            // COLUMN MAJOR
            buff[0] = 1; buff[4] = 0; buff[8]  = 0; buff[12] = 0;
            buff[1] = 0; buff[5] = 1; buff[9]  = 0; buff[13] = 0;
            buff[2] = 0; buff[6] = 0; buff[10] = 1; buff[14] = 0;
            buff[3] = 0; buff[7] = 0; buff[11] = 0; buff[15] = 1;
        }

        LMat4( const LMat4& other )
        {
            for ( int i = 0; i < 16; i++ )
            {
                buff[i] = other.buff[i];
            }
        }

        void set( int row, int col, GLfloat val )
        {
            buff[row + col * 4] = val;
        }

        GLfloat get( int row, int col ) const
        {
            return buff[row + col * 4];
        }

        LMat4 operator* ( const LMat4& other )
        {
            LMat4 _res;

            for ( int i = 0; i < 4; i++ )
            {
                for ( int j = 0; j < 4; j++ )
                {
                    _res.buff[ i + j * 4 ] = 0.0f;

                    for ( int k = 0; k < 4; k++ )
                    {
                        // Matrices are stored in column major form, so ...
                        // we use this indexing for the multiplication
                        // k + 4 * (fixed) -> over column
                        // (fixed) + 4 * k -> over row
                        _res.buff[ i + j * 4 ] += this->buff[ i + k * 4 ] * 
                                                  other.buff[ k + j * 4 ];
                    }
                }
            }

            return _res;
        }

        LMat4 extractRotation() const
        {
            LMat4 _res;

            _res.buff[0] = buff[0];
            _res.buff[1] = buff[1];
            _res.buff[2] = buff[2];

            _res.buff[4] = buff[4];
            _res.buff[5] = buff[5];
            _res.buff[6] = buff[6];

            _res.buff[8] = buff[8];
            _res.buff[9] = buff[9];
            _res.buff[10] = buff[10];
            
            return _res;
        }

        LMat4 inverse() const
        {
            LMat4 _res;

            float _r11 = get( 0, 0 ); float _r12 = get( 0, 1 );
            float _r21 = get( 1, 0 ); float _r22 = get( 1, 1 );
            float _r31 = get( 2, 0 ); float _r32 = get( 2, 1 );
            float _r41 = get( 3, 0 ); float _r42 = get( 3, 1 );

            float _r13 = get( 0, 2 ); float _r14 = get( 0, 3 );
            float _r23 = get( 1, 2 ); float _r24 = get( 1, 3 );
            float _r33 = get( 2, 2 ); float _r34 = get( 2, 3 );
            float _r43 = get( 3, 2 ); float _r44 = get( 3, 3 );

            // Calculate some intermediate values - "minors" of order 2
            float _m3434 = _r33 * _r44 - _r43 * _r34;
            float _m2434 = _r23 * _r44 - _r43 * _r24;
            float _m1434 = _r13 * _r44 - _r43 * _r14;
            float _m2334 = _r23 * _r34 - _r33 * _r24;
            float _m1334 = _r13 * _r34 - _r33 * _r14;
            float _m1234 = _r13 * _r24 - _r23 * _r14;

            float _m2312 = _r21 * _r32 - _r31 * _r22;
            float _m2412 = _r21 * _r42 - _r41 * _r22;
            float _m3412 = _r31 * _r42 - _r41 * _r32;
            float _m1312 = _r11 * _r32 - _r31 * _r12;
            float _m1412 = _r11 * _r42 - _r41 * _r12;
            float _m1212 = _r11 * _r22 - _r21 * _r12;        

            float _det = _r11 * ( _r22 * _m3434 - _r32 * _m2434 + _r42 * _m2334 ) -
                         _r21 * ( _r12 * _m3434 - _r32 * _m1434 + _r42 * _m1334 ) +
                         _r31 * ( _r12 * _m2434 - _r22 * _m1434 + _r42 * _m1234 ) - 
                         _r41 * ( _r12 * _m2334 - _r22 * _m1334 + _r32 * _m1234 );

            float _invdet = 1 / _det;

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

        static LMat4 perspective( float fov, float aspect, float zNear, float zFar )
        {
            LMat4 _res;

            float _ht = tan( ( fov / 2.0f ) * ( M_PI / 180.0f ) );

            _res.buff[0] = 1 / ( _ht * aspect )  ; _res.buff[4] =    0      ; _res.buff[8]  =                   0                      ; _res.buff[12] =  0;
            _res.buff[1] =          0            ; _res.buff[5] = 1 / _ht   ; _res.buff[9]  =                   0                      ; _res.buff[13] =  0;
            _res.buff[2] =          0            ; _res.buff[6] =    0      ; _res.buff[10] = -( zFar + zNear ) / ( zFar - zNear )     ; _res.buff[14] = -2.0f * ( zFar * zNear ) / ( zFar - zNear );
            _res.buff[3] =          0            ; _res.buff[7] =    0      ; _res.buff[11] =                  -1                      ; _res.buff[15] =  0;

            return _res;
        }

        static LMat4 ortho( float width, float height, float zNear, float zFar )
        {
            LMat4 _res;

            _res.buff[0] =  1 / ( width / 2 )   ; _res.buff[4] =       0            ; _res.buff[8]  =         0             ; _res.buff[12] = 0;
            _res.buff[1] =        0             ; _res.buff[5] = 1 / ( height / 2 ) ; _res.buff[9]  =         0             ; _res.buff[13] = 0;
            _res.buff[2] =        0             ; _res.buff[6] =       0            ; _res.buff[10] = -2 / ( zFar - zNear ) ; _res.buff[14] = -( zFar + zNear ) / ( zFar - zNear );
            _res.buff[3] =        0             ; _res.buff[7] =       0            ; _res.buff[11] =         0             ; _res.buff[15] =  1;        

            return _res;
        }

        static LMat4 lookAt( const LVec3& position, const LVec3& target, const LVec3& worldUp )
        {
            auto _front = LVec3::normalize( position - target );
            auto _right = LVec3::normalize( LVec3::cross( worldUp, _front ) );
            auto _up    = LVec3::normalize( LVec3::cross( _front, _right ) );

            LMat4 _res;

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

            _res.buff[12] = -LVec3::dot( _right, position );
            _res.buff[13] = -LVec3::dot( _up, position );
            _res.buff[14] = -LVec3::dot( _front, position );
            _res.buff[15] = 1;

            return _res;
        }

        static LMat4 scale( const LVec3& v )
        {
            LMat4 _res;

            _res.buff[0]  = v.x;
            _res.buff[5]  = v.y;
            _res.buff[10] = v.z;

            return _res;
        }

        static LMat4 translate( const LVec3& v )
        {
            LMat4 _res;

            _res.buff[12] = v.x;
            _res.buff[13] = v.y;
            _res.buff[14] = v.z;

            return _res;
        }

        static LMat4 rotationX( float theta )
        {
            LMat4 _res;

            _res.buff[5] = cos( theta );
            _res.buff[6] = sin( theta );
            _res.buff[9] = -sin( theta );
            _res.buff[10] = cos( theta );

            return _res;
        }

        static LMat4 rotationY( float theta )
        {
            LMat4 _res;

            _res.buff[0] = cos( theta );
            _res.buff[2] = -sin( theta );
            _res.buff[8] = sin( theta );
            _res.buff[10] = cos( theta );

            return _res;
        }

        static LMat4 rotationZ( float theta )
        {
            LMat4 _res;

            _res.buff[0] = cos( theta );
            _res.buff[1] = sin( theta );
            _res.buff[4] = -sin( theta );
            _res.buff[5] = cos( theta );

            return _res;
        }

        string toString()
        {
            string _res;

            for ( int i = 0; i < 16; i++ )
            {
                _res += to_string( i ) + string( ": " ) + to_string( buff[i] ) + string( "\n\r" );
            }

            return _res;
        }

    };


    struct LTri
    {
        GLint i1;
        GLint i2;
        GLint i3;
    };

    union LInd3
    {
        GLint buff[3];
        LTri tri;

        LInd3()
        {
            buff[0] = 0;
            buff[1] = 0;
            buff[2] = 0;
        }

        LInd3( GLint v1, GLint v2, GLint v3 )
        {
            buff[0] = v1;
            buff[1] = v2;
            buff[2] = v3;
        }

        void dump()
        {
            cout << "i1: " << buff[0] << " - i2: " << buff[1] << " - i3: " << buff[2] << endl;
        }
    };

    struct LInd2
    {

        int i;
        int j;

        LInd2()
        {
            i = 0;
            j = 0;
        }

        LInd2( int i, int j )
        {
            this->i = i;
            this->j = j;
        }

    };

    struct LVec2
    {
        GLfloat x;
        GLfloat y;

        LVec2()
        {
            this->x = 0.0f;
            this->y = 0.0f;
        }

        LVec2( GLfloat x, GLfloat y )
        {
            this->x = x;
            this->y = y;
        }
    };

    struct LVec4
    {
        GLfloat x;
        GLfloat y;
        GLfloat z;
        GLfloat w;

        LVec4()
        {
            this->x = 0.0f;
            this->y = 0.0f;
            this->z = 0.0f;
            this->w = 0.0f;
        }

        LVec4( GLfloat x, GLfloat y, GLfloat z, GLfloat w )
        {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
        }

        LVec4( const LVec3& vec3, GLfloat w )
        {
            this->x = vec3.x;
            this->y = vec3.y;
            this->z = vec3.z;
            this->w = w;
        }
    };

    LVec4 operator* ( const LMat4& mat, const LVec4& vec );

    float toRadians( float angle );

    struct LTextureData
    {
        u8* data;
        int width;
        int height;
        int channels;
        int ftype;
    };

    struct LCubeTextureData
    {
        u8* vpData[6];
        int width;
        int height;
        int channels;
        int ftype;
    };

    struct LLDirUniforms
    {
        GLuint direction;
        GLuint ambient;
        GLuint diffuse;
        GLuint specular;
        GLuint isActive;
    };

    struct LMaterialUniforms
    {
        GLuint ambient;
        GLuint diffuse;
        GLuint specular;
        GLuint shininess;
    };

}