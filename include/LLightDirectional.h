
#pragma once

#include "LILight.h"

namespace engine
{

    class LLightDirectional : public LILight
    {

        public :

        LVec3 direction;

        LLightDirectional( const LVec3& ambient, 
                           const LVec3& diffuse,
                           const LVec3& specular, 
                           int lIndx, 
                           const LVec3& direction ) 
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