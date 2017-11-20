
#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>

#define USE_MODERN_OPENGL 1

using namespace std;

namespace miniengine
{

    template<class T>
    struct LVec3T
    {
        T x;
        T y;
        T z;

        LVec3T()
        {
            this->x = 0;
            this->y = 0;
            this->z = 0;        
        }

        LVec3T( T x, T y, T z )
        {
            this->x = x;
            this->y = y;
            this->z = z;
        }
        
        LVec3T<T> operator+( const LVec3T<T>& other )
        {
            return LVec3T<T>( this->x + other.x,
                              this->y + other.y,
                              this->z + other.z );
        }

        LVec3T<T> operator-( const LVec3T<T>& other )
        {
            return LVec3T<T>( this->x - other.x,
                              this->y - other.y,
                              this->z - other.z );
        }

        void normalize()
        {
            T _len = sqrt( x * x + y * y + z * z );
            x = x / _len;
            y = y / _len;
            z = z / _len;
        }

        LVec3T<T> operator*( float s )
        {
            return LVec3T<T>( this->x * s,
                              this->y * s,
                              this->z * s );
        }

        void scale( float sx, float sy, float sz )
        {
            x *= sx; y *= sy; z *= sz;
        }

        static LVec3T<T> cross( const LVec3T<T>& v1,
                                const LVec3T<T>& v2 )
        {
            LVec3T<T> _res;

            _res.x = v1.y * v2.z - v2.y * v1.z;
            _res.y = -v1.x * v2.z + v2.x * v1.z;
            _res.z = v1.x * v2.y - v2.x * v1.y;

            return _res;
        }

    };

    template<class T>
    struct LTriT
    {
        T i1;
        T i2;
        T i3;
    };

    template<class T>
    union LInd3T
    {
        T buff[3];
        LTriT<T> tri;

        LInd3T()
        {

        }

        LInd3T( T v1, T v2, T v3 )
        {
            buff[0] = v1;
            buff[1] = v2;
            buff[2] = v3;
        }
    };


    typedef LVec3T<GLfloat> LVec3;
    typedef LInd3T<GLuint> LInd3;

    struct LLightProps
    {
        LVec3 ambient;
        LVec3 diffuse;
        LVec3 specular;

        LVec3 pos;
        LVec3 dir;
    };

    struct LRenderInfo
    {
        glm::mat4 viewMat;
        glm::mat4 projMat;

        vector<LLightProps> lights;
    };
}