
#pragma once

#include "LCommon.h"

#define FOG_TYPE_LINEAR     0
#define FOG_TYPE_EXP        1
#define FOG_TYPE_EXP_SQUARE 2

namespace engine
{

    struct LFog
    {

        GLint type;
        LVec4 color;
        GLfloat density;

        GLfloat start;
        GLfloat end;

        GLint isActive;

        LFog( GLint type, LVec4 color, GLfloat density, GLfloat start, GLfloat end )
        {
            this->type = type;
            this->color = color;
            this->density = density;

            this->start = start;
            this->end = end;

            this->isActive = 1;
        }

        ~LFog()
        {

        }

    };


}