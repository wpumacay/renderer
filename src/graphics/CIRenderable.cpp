
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
        m_name      = name;
        m_type      = eRenderableType::BASE;
        m_visible   = true;
        m_wireframe = false;
        m_material  = std::unique_ptr< CPhongMaterial >( new CPhongMaterial( name + "_phong_mat",
                                                                             RENDERABLE_DEFAULT_AMBIENT_COLOR,
                                                                             RENDERABLE_DEFAULT_DIFFUSE_COLOR,
                                                                             RENDERABLE_DEFAULT_SPECULAR_COLOR,
                                                                             RENDERABLE_DEFAULT_SHININESS ) );

        scale       = { 1.0f, 1.0f, 1.0f }; // no scaling by default
        rotation    = CMat4(); // identity
        position    = { 0.0f, 0.0f, 0.0f }; // origin in model-space
    }

    CIRenderable::~CIRenderable()
    {
        // nothing to release manually
    }

    void CIRenderable::setMaterial( std::unique_ptr< CIMaterial > material ) 
    { 
        // grab ownership of this material
        m_material = std::move( material ); 
    }

    CMat4 CIRenderable::matModel() const
    {
        return CMat4::translate( position ) * rotation * CMat4::scale( scale );
    }

}