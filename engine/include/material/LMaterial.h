
#pragma once

#include "../LCommon.h"
#include "../gl/LTexture.h"

#define MAT_DEFAULT_PLAIN     LVec3( 0.0f, 1.0f, 0.0f )
#define MAT_DEFAULT_AMBIENT   LVec3( 1.0f, 1.0f, 1.0f )
#define MAT_DEFAULT_DIFFUSE   LVec3( 1.0f, 1.0f, 1.0f )
#define MAT_DEFAULT_SPECULAR  LVec3( 1.0f, 1.0f, 1.0f )
#define MAT_DEFAULT_SHININESS 20.0f

using namespace std;

namespace engine
{

    class LMaterial
    {
        private :

        // Component for plain color without lighting
        LVec3 m_plainColor;
        // Components for the phong model
        LVec3 m_ambient;
        LVec3 m_diffuse;
        LVec3 m_specular;
        float m_shininess;

        bool m_lightingEnabled;

        vector<LTexture*> m_textures;

        public :

        LMaterial();
        LMaterial( const LVec3& cAmbient, 
                   const LVec3& cDiffuse,
                   const LVec3& cSpecular,
                   const LVec3& cPlainColor,
                   float cShininess );

        ~LMaterial();

        void addTexture( LTexture* pTexture ) { m_textures.push_back( pTexture ); }

        void enableLighting() { m_lightingEnabled = true; }
        void disableLighting() { m_lightingEnabled = false; }

        void bind();
        void unbind();

    };

}


