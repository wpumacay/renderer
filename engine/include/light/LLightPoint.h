
#pragma once

#include "LILight.h"

namespace engine
{

    class LLightPoint : public LILight
    {

        private :

        LVec3 m_position;

        float m_attenuation_const;
        float m_attenuation_linear;
        float m_attenuation_quadratic;

        public :

        static int s_count;

        LLightPoint( const LVec3& ambient,
                     const LVec3& diffuse,
                     const LVec3& specular,
                     int lIndx,
                     const LVec3& position,
                     float attenuation_const,
                     float attenuation_linear,
                     float attenuation_quadratic );

        ~LLightPoint();

        void bind() override;
        void unbind() override;

    };



}