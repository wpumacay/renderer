
#pragma once

#include "LILight.h"

namespace engine
{

    class LLightDirectional : public LILight
    {

        public :

        CVec3 direction;

        LLightDirectional( const CVec3& ambient, 
                           const CVec3& diffuse,
                           const CVec3& specular, 
                           int lIndx, 
                           const CVec3& direction ) 
            : LILight( ambient, diffuse, specular, lIndx )
        {
            this->direction = direction;
            m_type = light::TYPE_DIRECTIONAL;
        }

        ~LLightDirectional()
        {

        }

        static light::_light getStaticType()
        {
            return light::TYPE_DIRECTIONAL;
        }

    };



}