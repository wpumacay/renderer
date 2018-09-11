
#pragma once

#include "LILight.h"

namespace engine
{

    class LLightSpot : public LILight
    {

        public :

        LVec3 position;
        LVec3 direction;

        float attenuation_constant;
        float attenuation_linear;
        float attenuation_quadratic;

        float cutOff;
        float outerCutOff;

        LLightSpot( const LVec3& ambient,
                    const LVec3& diffuse,
                    const LVec3& specular,
                    int lIndx,
                    const LVec3& position,
                    const LVec3& direction,
                    float attenuation_constant,
                    float attenuation_linear,
                    float attenuation_quadratic,
                    float cutOff, 
                    float outerCutOff )
            : LILight( ambient, diffuse, specular, lIndx )
        {
            this->position = position;
            this->direction = direction;

            this->attenuation_constant  = attenuation_constant;
            this->attenuation_linear    = attenuation_linear;
            this->attenuation_quadratic = attenuation_quadratic;

            this->cutOff      = cos( toRadians( cutOff ) );
            this->outerCutOff = cos( toRadians( outerCutOff ) );

            m_type = light::TYPE_SPOT;
        }

        ~LLightSpot()
        {

        }

        static light::_light getStaticType()
        {
            return light::TYPE_SPOT;
        }
    };



}