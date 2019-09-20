
#pragma once

#include "LILight.h"

namespace engine
{

    class LLightPoint : public LILight
    {

        public :

        CVec3 position;

        float attenuation_constant;
        float attenuation_linear;
        float attenuation_quadratic;

        LLightPoint( const CVec3& ambient, 
                     const CVec3& diffuse,
                     const CVec3& specular, 
                     int lIndx, 
                     const CVec3& position,
                     float attenuation_constant, 
                     float attenuation_linear,
                     float attenuation_quadratic )
            : LILight( ambient, diffuse, specular, lIndx )
        {
            this->position = position;

            this->attenuation_constant  = attenuation_constant;
            this->attenuation_linear    = attenuation_linear;
            this->attenuation_quadratic = attenuation_quadratic;

            m_type = light::TYPE_POSITIONAL;
        }

        ~LLightPoint()
        {

        }

        static light::_light getStaticType()
        {
            return light::TYPE_POSITIONAL;
        }

    };



}