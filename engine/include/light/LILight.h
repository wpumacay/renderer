

#pragma once

#include "LCommon.h"

namespace engine
{


    class LILight
    {

        protected :

        LVec3 m_ambient;
        LVec3 m_diffuse;
        LVec3 m_specular;

        int m_indx;

        string m_type;

        public :

        int active;

        LILight( const LVec3& ambient,
                 const LVec3& diffuse,
                 const LVec3& specular,
                 int lIndx );

        ~LILight();

        string getType() { return m_type; }

        virtual void bind() = 0;
        virtual void unbind() = 0;

    };




}