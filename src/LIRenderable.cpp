
#include "LIRenderable.h"

using namespace std;

namespace engine
{

    LIRenderable::LIRenderable()
    {
        m_type              = RENDERABLE_TYPE_BASE;
        m_isVisible         = true;
        m_drawAsWireframe   = false;
        this->debug         = false;
    }

    LIRenderable::~LIRenderable()
    {
        // we only hold references, the assetsmanager ...
        // should handle deletion of the textures
        m_textures.clear();
    }

    int LIRenderable::getType()
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

    LMat4 LIRenderable::getModelMatrix()
    {
        LMat4 _model;

        _model = LMat4::scale( scale ) * _model;
        _model = rotation * _model;
        _model = LMat4::translate( pos ) * _model;

        return _model;
    }

    void LIRenderable::addTexture( LTexture* pTexture )
    {
        m_textures.push_back( pTexture );
    }

    bool LIRenderable::hasTextures()
    {
        return m_textures.size() > 0;
    }

    vector< LTexture* > LIRenderable::textures()
    {
        return m_textures;
    }

    bool LIRenderable::isVisible()
    {
        return m_isVisible;
    }

    void LIRenderable::setVisibility( bool visibility ) 
    { 
        m_isVisible = visibility; 
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