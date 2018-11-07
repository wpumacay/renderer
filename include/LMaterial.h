
#pragma once

#include "LCommon.h"

#define MAT_DEFAULT_AMBIENT   LVec3( 0.0f, 1.0f, 0.0f )
#define MAT_DEFAULT_DIFFUSE   LVec3( 0.0f, 1.0f, 0.0f )
#define MAT_DEFAULT_SPECULAR  LVec3( 0.0f, 1.0f, 0.0f )
#define MAT_DEFAULT_SHININESS 20.0f

using namespace std;

namespace engine
{

    struct LMaterial
    {
        
        LVec3 ambient;
        LVec3 diffuse;
        LVec3 specular;
        float shininess;

        LMaterial()
        {
            this->ambient    = MAT_DEFAULT_AMBIENT;
            this->diffuse    = MAT_DEFAULT_DIFFUSE;
            this->specular   = MAT_DEFAULT_SPECULAR;
            this->shininess  = MAT_DEFAULT_SHININESS;
        }

        LMaterial( const LVec3& cAmbient, 
                   const LVec3& cDiffuse,
                   const LVec3& cSpecular,
                   float cShininess )
        {
            this->ambient   = cAmbient;
            this->diffuse   = cDiffuse;
            this->specular  = cSpecular;
            this->shininess = cShininess;
        }

        LVec3 getColor() { return this->ambient; }
        void setColor( const LVec3& color )
        {
            this->ambient = color;
            this->diffuse = color;
            this->specular = color;
        }
        void setColor( float r, float g, float b )
        {
            LVec3 _color( r, g, b );
            setColor( _color );
        }
    };

}


