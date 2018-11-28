

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
 
        int m_type;
        bool m_isVisible;
        LMaterial* m_material;
        vector< LTexture* > m_textures;

        public :

        LVec3 pos;
        LMat4 rotation;
        LVec3 scale;

        LIRenderable() { m_isVisible = true; }

        int getType() { return m_type; }

        void setMaterial( LMaterial* pMaterial )
        {
            if ( m_material != NULL )
            {
                delete m_material;
            }
            
            m_material = pMaterial;
        }

        LMaterial* getMaterial() { return m_material; }

        LMat4 getModelMatrix()
        {
            LMat4 _model;

            _model = LMat4::scale( scale ) * _model;
            _model = rotation * _model;
            _model = LMat4::translate( pos ) * _model;

            return _model;
        }

        void addTexture( LTexture* pTexture ) { m_textures.push_back( pTexture ); }
        bool hasTextures() { return m_textures.size() > 0; }
        vector< LTexture* > textures() { return m_textures; }

        bool isVisible() { return m_isVisible; }
        void setVisibility( bool visibility ) { m_isVisible = visibility; }

        virtual void render() = 0;
    };

}