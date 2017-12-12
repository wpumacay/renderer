
#pragma once


#include "LILight.h"

namespace engine
{

    class LLightSpot : public LILight
    {

        private :

        LVec3 m_position;
        LVec3 m_direction;

        float m_attenuation_const;
        float m_attenuation_linear;
        float m_attenuation_quadratic;

        float m_cutOff;
        float m_outerCutOff;

        public :

        static int s_count;

        LLightSpot( const LVec3& ambient,
                    const LVec3& diffuse,
                    const LVec3& specular,
                    int lIndx,
                    const LVec3& position,
                    const LVec3& direction,
                    float attenuation_const,
                    float attenuation_linear,
                    float attenuation_quadratic,
                    float cutOff, float outerCutOff );

        ~LLightSpot();

        void bind() override;
        void unbind() override;

    };



}