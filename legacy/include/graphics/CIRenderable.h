#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <materials/CMaterial.h>

namespace engine
{

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
        CMat3 rotation;
        CVec3 scale;

        bool castsShadows;
        bool receivesShadows;
        bool cullFaces;

        CIRenderable( const std::string& name );
        virtual ~CIRenderable();

        void render();

        void setBoundExtents( const CVec3& extents ) { m_boundExtents = extents; }
        void setBoundCenter( const CVec3& center ) { m_boundCenter = center; }
        void setMaterial( std::unique_ptr< CMaterial > material ) { m_material = std::move( material ); }
        void setVisibility( bool visibility ) { m_visible = visibility; }
        void setWireframe( bool wireframe ) { m_wireframe = wireframe; }

        CMat4 matModel() const;
        std::string name() const { return m_name; }
        eRenderableType type() const { return m_type; };
        CMaterial* material() const { return m_material.get(); }
        CVec3 boundExtents() const { return m_boundExtents; }
        bool visible() const { return m_visible; }
        bool wireframe() const { return m_wireframe; };

        CBoundingBox bbox() const;
        CBoundingSphere bsphere() const;

        void setMaskId( int32 maskId ) { m_maskId = maskId; }
        void setObjectId( int32 objectId ) { m_objectId = objectId; }

        int32 maskId() const { return m_maskId; }
        int32 objectId() const { return m_objectId; }

        static eRenderableType GetStaticType() { return eRenderableType::BASE; }

    protected :

        virtual void _renderInternal() = 0;

    protected :

        bool                            m_visible;
        bool                            m_wireframe;
        std::string                     m_name;
        eRenderableType                 m_type;
        CVec3                           m_boundExtents;
        CVec3                           m_boundCenter;
        std::unique_ptr< CMaterial >    m_material;
        int32                           m_maskId; // used for semantic_only rendering
        int32                           m_objectId; // used for object-picking

    };

}
