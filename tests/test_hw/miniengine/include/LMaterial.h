
#pragma once

#include "../LMiniCommon.h"


namespace miniengine
{


    struct LMaterial
    {

        LVec3 ambient;
        LVec3 diffuse;
        LVec3 specular;

        float shininess;

        LMaterial()
        {
            ambient = LVec3( 1.0f, 0.0f, 0.0f );
            diffuse = LVec3( 0.5f, 0.0f, 0.0f );
            specular = LVec3( 0.5f, 0.0f, 0.0f );
            shininess = 20.0f;
        }
    };




}


