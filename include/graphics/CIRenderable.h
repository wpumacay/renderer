#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>
#include <materials/CIMaterial.h>
#include <materials/CLambertMaterial.h>
#include <materials/CPhongMaterial.h>

namespace engine
{

    const CVec3 RENDERABLE_DEFAULT_AMBIENT_COLOR = { 0.5f, 0.5f, 0.5f };
    const CVec3 RENDERABLE_DEFAULT_DIFFUSE_COLOR = { 0.5f, 0.5f, 0.5f };
    const CVec3 RENDERABLE_DEFAULT_SPECULAR_COLOR = { 0.5f, 0.5f, 0.5f };
    const float32 RENDERABLE_DEFAULT_SHININESS = 64.0f;

    enum class eRenderableType
    {
        BASE = 0,
        MESH,
        MODEL
    };

    std::string toString( const eRenderableType& renderable );

    class CIRenderable
    {

    public :

        CVec3 position;
        CMat4 rotation;
        CVec3 scale;

        CIRenderable( const std::string& name );
        virtual ~CIRenderable();

        virtual void render() = 0;

        void setMaterial( std::unique_ptr< CIMaterial > material );
        void setVisibility( bool visibility ) { m_visible = visibility; }
        void setWireframe( bool wireframe ) { m_wireframe = wireframe; }
        void setCastShadow( bool castShadow ) { m_castShadow = castShadow; }

        CMat4 matModel() const;
        std::string name() const { return m_name; }
        eRenderableType type() const { return m_type; };
        CIMaterial* material() const { return m_material.get(); }
        bool visible() const { return m_visible; }
        bool wireframe() const { return m_wireframe; };
        bool castShadow() const { return m_castShadow; }

    protected :

        bool                            m_visible;
        bool                            m_wireframe;
        bool                            m_castShadow;
        std::string                     m_name;
        eRenderableType                 m_type;
        std::unique_ptr< CIMaterial >   m_material;

    };

}