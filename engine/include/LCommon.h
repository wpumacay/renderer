
#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <iostream>
#include <string>
#include <vector>

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

using namespace std;

#define _PI 3.141592653589793

typedef unsigned char u8;

typedef void ( *FnPtr_keyboard_callback )( int key, int action );
typedef void ( *FnPtr_mouse_callback )( int button, int action, double x, double y );

namespace engine
{

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

    struct LVec2
    {
        GLfloat x;
        GLfloat y;

        LVec2()
        {
            this->x = 0;
            this->y = 0;
        }

        LVec2( GLfloat x, GLfloat y )
        {
            this->x = x;
            this->y = y;
        }
    };

    struct LTri
    {
        GLuint i1;
        GLuint i2;
        GLuint i3;
    };

    union LInd3
    {
        GLuint buff[3];
        LTri tri;

        LInd3()
        {
            buff[0] = 0;
            buff[1] = 0;
            buff[2] = 0;
        }

        LInd3( GLuint v1, GLuint v2, GLuint v3 )
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

    struct LTextureData
    {
        u8* data;
        int width;
        int height;
        int channels;
    };

    struct LCubeTextureData
    {
        u8* vpData[6];
        int width;
        int height;
        int channels;
    };

    float toRadians( float angle );


}