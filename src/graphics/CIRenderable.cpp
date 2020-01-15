
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
        m_boundExtents  = CVec3( 0.0f, 0.0f, 0.0f );
        m_material      = std::unique_ptr< CMaterial >( new CMaterial( name + "_mat", eMaterialType::PHONG ) );

        scale       = CVec3( 1.0f, 1.0f, 1.0f ); // no scaling by default
        rotation    = CMat3(); // identity
        position    = CVec3( 0.0f, 0.0f, 0.0f ); // origin in model-space

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
        return engine::translation( position ) * engine::rotate( rotation ) * engine::scale( scale );
    }

    CBoundingBox CIRenderable::bbox() const
    {
        return { m_boundExtents.scaled( scale ),
                 engine::translation( position ) * engine::rotate( rotation ) * engine::translation(  m_boundCenter.scaled( scale ) ) };
    }

    CBoundingSphere CIRenderable::bsphere() const
    {
        return { 0.5f * m_boundExtents.scaled( scale ).length(), 
                 position + CVec3( engine::rotate( rotation ) * CVec4( m_boundCenter.scaled( scale ), 1.0f ) ) };
    }

}