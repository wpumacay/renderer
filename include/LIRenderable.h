

#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <LMaterial.h>

#include <core/CTexture.h>

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

    public :

        CVec3 pos;
        CMat4 rotation;
        CVec3 scale;

        bool debug;

        LIRenderable();
        virtual ~LIRenderable();

        eRenderableType getType();

        void setMaterial( LMaterial* pMaterial );
        LMaterial* getMaterial();

        CMat4 getModelMatrix();

        void setTexture( CTexture* texture ) { m_texture = texture; }
        CTexture* texture() const { return m_texture; }

        bool isVisible() const { return m_isVisible; }
        void setVisibility( bool visibility ) { m_isVisible = visibility; }

        virtual void setWireframeMode( bool useWireframe );
        bool isWireframe();

        virtual void render() = 0;

    protected :

        bool            m_isVisible;
        bool            m_drawAsWireframe;
        LMaterial*      m_material;
        CTexture*       m_texture;
        eRenderableType m_type;

    };

}