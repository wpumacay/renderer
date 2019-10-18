
#include <graphics/CIRenderable.h>

namespace engine
{

    std::string toString( const eRenderableType& type )
    {
        if ( type == eRenderableType::MESH ) return "mesh";
        if ( type == eRenderableType::MODEL ) return "model";

        ENGINE_CORE_ASSERT( false, "Invalid eRenderableType enum given" );

        return "undefined";
    }

    CIRenderable::CIRenderable( const std::string& name )
    {
        m_name          = name;
        m_type          = eRenderableType::BASE;
        m_visible       = true;
        m_wireframe     = false;
        m_boundExtents  = { 0.0f, 0.0f, 0.0f };
        m_material      = std::unique_ptr< CMaterial >( new CMaterial( name + "_mat", eMaterialType::PHONG ) );

        scale       = { 1.0f, 1.0f, 1.0f }; // no scaling by default
        rotation    = CMat4(); // identity
        position    = { 0.0f, 0.0f, 0.0f }; // origin in model-space

        castsShadows = true;
        receivesShadows = true;
        cullFaces = false;
    }

    CIRenderable::~CIRenderable()
    {
        // nothing to release manually
    }

    CMat4 CIRenderable::matModel() const
    {
        return CMat4::translation( position ) * rotation * CMat4::scale( scale );
    }

    CBoundingBox CIRenderable::bbox() const
    {
        return { CVec3::scale( m_boundExtents, scale ),
                 CMat4::translation( position ) * rotation * CMat4::translation(  CVec3::scale( m_boundCenter, scale ) ) };
    }

    CBoundingSphere CIRenderable::bsphere() const
    {
        return { CVec3::length( CVec3::scale( 0.5f * m_boundExtents, scale ) ), 
                 position + CVec3( rotation * CVec4( CVec3::scale( m_boundCenter, scale ), 1.0f ) ) };
    }

}