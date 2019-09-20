

#pragma once

#include <CCommon.h>
#include <CMath.h>

namespace engine
{

    namespace light
    {
        enum _light
        {
            TYPE_BASE,
            TYPE_DIRECTIONAL,
            TYPE_POSITIONAL,
            TYPE_SPOT
        };
    }


    class LILight
    {

        protected :

        int m_indx;
        light::_light m_type;

        CVec3 m_virtualPosition;

        public :

        CVec3 ambient;
        CVec3 diffuse;
        CVec3 specular;

        int isActive;

        LILight( const CVec3& ambient,
                 const CVec3& diffuse,
                 const CVec3& specular,
                 int lIndx )
        {
            this->ambient = ambient;
            this->diffuse = diffuse;
            this->specular = specular;
            this->isActive = 1;
            
            m_indx = lIndx;
        }

        ~LILight()
        {
            
        }

        static light::_light getStaticType()
        {
            return light::TYPE_BASE;
        }

        light::_light getType() { return m_type; }
        int getIndx() { return m_indx; }

        void setVirtualPosition( const CVec3& virtualPos ) { m_virtualPosition = virtualPos; }
        CVec3 getVirtualPosition() { return m_virtualPosition; }

    };




}