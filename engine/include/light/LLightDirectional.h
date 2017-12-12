
#pragma once

#include "LILight.h"

namespace engine
{

    class LLightDirectional : public LILight
    {

        private :

        LVec3 m_direction;


        public :

        static int s_count;

        LLightDirectional( const LVec3& ambient,
                           const LVec3& diffuse,
                           const LVec3& specular,
                           int lIndx,
                           const LVec3& direction );

        ~LLightDirectional();

        void bind() override;
        void unbind() override;

    };



}