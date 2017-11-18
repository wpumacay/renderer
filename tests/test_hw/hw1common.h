
#pragma once


#define USE_MODERN_OPENGL 1

namespace miniengine
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
    };






}