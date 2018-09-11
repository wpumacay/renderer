

#pragma once

#include "LCommon.h"
#include "LMaterial.h"
#include "LTexture.h"

using namespace std;

namespace engine
{

    class LIRenderable
    {
        protected :
 
        int m_type;
        LMaterial* m_material;
        vector< LTexture* > m_textures;

        public :

        LVec3 pos;
        glm::mat4 rotation;
        LVec3 scale;

        int getType() { return m_type; }

        void setMaterial( LMaterial* pMaterial )
        {
            if ( m_material != NULL )
            {
                delete m_material;
            }
            
            m_material = pMaterial;
        }

        LMaterial* getMaterial() 
        { 
            return m_material; 
        }

        void addTexture( LTexture* pTexture )
        {
            m_textures.push_back( pTexture );
        }

        int hasTextures() { return m_textures.size(); }

        virtual void render() = 0;
    };

}