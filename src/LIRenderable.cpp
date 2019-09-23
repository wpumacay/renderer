
#include <LIRenderable.h>

namespace engine
{

    LIRenderable::LIRenderable()
    {
        m_type              = eRenderableType::BASE;
        m_isVisible         = true;
        m_drawAsWireframe   = false;
        m_texture           = nullptr;
        debug               = false;
    }

    LIRenderable::~LIRenderable()
    {
        // nothing to do here for now
    }

    eRenderableType LIRenderable::getType()
    {
        return m_type;
    }

    void LIRenderable::setMaterial( LMaterial* pMaterial )
    {
        if ( m_material )
        {
            delete m_material;
        }

        m_material = pMaterial;
    }

    LMaterial* LIRenderable::getMaterial()
    {
        return m_material;
    }

    CMat4 LIRenderable::getModelMatrix()
    {
        CMat4 _model;

        _model = CMat4::scale( scale ) * _model;
        _model = rotation * _model;
        _model = CMat4::translate( pos ) * _model;

        return _model;
    }

    void LIRenderable::setWireframeMode( bool useWireframe )
    {
        m_drawAsWireframe = useWireframe;
    }

    bool LIRenderable::isWireframe()
    {
        return m_drawAsWireframe;
    }

}