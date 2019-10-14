
#include <materials/CPhongMaterial.h>

namespace engine
{

    CPhongMaterial::CPhongMaterial( const std::string& name,
                                    const CVec3& ambientColor,
                                    const CVec3& diffuseColor,
                                    const CVec3& specularColor,
                                    float32 specularShininess,
                                    CTexture* albedoMap,
                                    CTexture* specularMap,
                                    CTexture* normalMap )
        : CIMaterial( name, 
                      ambientColor, diffuseColor, specularColor, specularShininess,
                      albedoMap, specularMap, normalMap )
    {
        m_type = eMaterialType::PHONG;
    }

    CPhongMaterial::~CPhongMaterial()
    {
        // do nothing for now
    }

    void CPhongMaterial::bind( CShader* shaderPtr )
    {
        shaderPtr->setVec3( "u_material.ambient", ambient );
        shaderPtr->setVec3( "u_material.diffuse", diffuse );
        shaderPtr->setVec3( "u_material.specular", specular );
        shaderPtr->setFloat( "u_material.shininess", shininess );

        if ( m_albedoMap )
        {
            shaderPtr->setInt( "u_material.albedoMap", 0 );
            shaderPtr->setInt( "u_material.albedoMapActive", 1 );
            glActiveTexture( GL_TEXTURE0 );
            m_albedoMap->bind();
        }
        else
        {
            shaderPtr->setInt( "u_material.albedoMapActive", 0 );
        }

        if ( m_specularMap )
        {
            shaderPtr->setInt( "u_material.specularMap", 1 );
            shaderPtr->setInt( "u_material.specularMapActive", 1 );
            glActiveTexture( GL_TEXTURE1 );
            m_specularMap->bind();
        }
        else
        {
            shaderPtr->setInt( "u_material.specularMapActive", 0 );
        }

        if ( m_normalMap )
        {
            shaderPtr->setInt( "u_material.normalMap", 2 );
            shaderPtr->setInt( "u_material.normalMapActive", 1 );
            glActiveTexture( GL_TEXTURE2 );
            m_normalMap->bind();
        }
        else
        {
            shaderPtr->setInt( "u_material.normalMapActive", 0 );
        }
    }

    void CPhongMaterial::unbind()
    {
        if ( m_albedoMap )
            m_albedoMap->unbind();

        if ( m_specularMap )
            m_specularMap->unbind();

        if ( m_normalMap )
            m_normalMap->unbind();

        glActiveTexture( GL_TEXTURE0 );
    }

    std::string CPhongMaterial::_toStringInternal()
    {
        std::string _strRep;

        _strRep += "ambient     : " + engine::toString( ambient ) + "\n\r";
        _strRep += "diffuse     : " + engine::toString( diffuse ) + "\n\r";
        _strRep += "specular    : " + engine::toString( specular ) + "\n\r";
        _strRep += "shininess   : " + std::to_string( shininess ) + "\n\r";
        _strRep += "albedoMap  : " + ( ( m_albedoMap ) ? m_albedoMap->name() : "none" ) + "\n\r";
        _strRep += "specularMap : " + ( ( m_specularMap ) ? m_specularMap->name() : "none" ) + "\n\r";
        _strRep += "normalMap   : " + ( ( m_normalMap ) ? m_normalMap->name() : "none" ) + "\n\r";

        return _strRep;
    }

}