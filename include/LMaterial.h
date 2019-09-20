
#pragma once

#include <CCommon.h>
#include <CMath.h>

#define MAT_DEFAULT_AMBIENT   CVec3( 0.0f, 1.0f, 0.0f )
#define MAT_DEFAULT_DIFFUSE   CVec3( 0.0f, 1.0f, 0.0f )
#define MAT_DEFAULT_SPECULAR  CVec3( 0.0f, 1.0f, 0.0f )
#define MAT_DEFAULT_SHININESS 20.0f

using namespace std;

namespace engine
{

    struct LMaterial
    {
        
        CVec3 ambient;
        CVec3 diffuse;
        CVec3 specular;
        float shininess;

        LMaterial()
        {
            this->ambient    = MAT_DEFAULT_AMBIENT;
            this->diffuse    = MAT_DEFAULT_DIFFUSE;
            this->specular   = MAT_DEFAULT_SPECULAR;
            this->shininess  = MAT_DEFAULT_SHININESS;
        }

        LMaterial( const CVec3& cAmbient, 
                   const CVec3& cDiffuse,
                   const CVec3& cSpecular,
                   float cShininess )
        {
            this->ambient   = cAmbient;
            this->diffuse   = cDiffuse;
            this->specular  = cSpecular;
            this->shininess = cShininess;
        }

        CVec3 getColor() { return this->ambient; }
        void setColor( const CVec3& color )
        {
            this->ambient = color;
            this->diffuse = color;
            this->specular = color;
        }
        void setColor( float r, float g, float b )
        {
            CVec3 _color( r, g, b );
            setColor( _color );
        }
    };

}


