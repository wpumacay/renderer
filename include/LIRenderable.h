

#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <LMaterial.h>
#include <LTexture.h>

using namespace std;

namespace engine
{

    enum class eRenderableType
    {
        BASE = 0,
        MESH,
        MODEL
    };

    std::string toString( const eRenderableType& renderable );

    class LIRenderable
    {
        protected :

        eRenderableType     m_type;
        bool                m_isVisible;
        bool                m_drawAsWireframe;
        LMaterial*          m_material;
        vector< LTexture* > m_textures;

        public :

        CVec3 pos;
        CMat4 rotation;
        CVec3 scale;

        bool debug;

        LIRenderable();
        ~LIRenderable();

        eRenderableType getType();

        void setMaterial( LMaterial* pMaterial );
        LMaterial* getMaterial();

        CMat4 getModelMatrix();

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