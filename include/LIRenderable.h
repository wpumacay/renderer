

#pragma once

#include <LCommon.h>
#include <LMaterial.h>
#include <LTexture.h>

using namespace std;

namespace engine
{

    class LIRenderable
    {
        protected :
 
        int                 m_type;
        bool                m_isVisible;
        bool                m_drawAsWireframe;
        LMaterial*          m_material;
        vector< LTexture* > m_textures;

        public :

        LVec3 pos;
        LMat4 rotation;
        LVec3 scale;

        bool debug;

        LIRenderable();
        ~LIRenderable();

        int getType();

        void setMaterial( LMaterial* pMaterial );
        LMaterial* getMaterial();

        LMat4 getModelMatrix();

        void addTexture( LTexture* pTexture );
        bool hasTextures();
        vector< LTexture* > textures();

        bool isVisible();
        void setVisibility( bool visibility );

        virtual void setWireframeMode( bool useWireframe );
        bool isWireframe();

        virtual void render() = 0;
    };

}