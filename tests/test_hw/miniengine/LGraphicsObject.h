
#pragma once

#include "hw1common.h"


namespace miniengine
{



    class LGraphicsObject
    {

        protected :

        LVec3 m_color;

        public :

        LVec3 scale;
        LVec3 rot;
        LVec3 pos;

        LGraphicsObject()
        {
            scale.x = 1.0f;
            scale.y = 1.0f;
            scale.z = 1.0f;
        }

        

    };











}