
#include <materials/CPhongMaterial.h>

namespace engine
{

    CPhongMaterial::CPhongMaterial( const std::string& name,
                                    const CVec3& ambientColor,
                                    const CVec3& diffuseColor,
                                    const CVec3& specularColor,
                                    float32 specularShininess,
                                    std::shared_ptr< CTexture > albedoMap,
                                    std::shared_ptr< CTexture > specularMap,
                                    std::shared_ptr< CTexture > normalMap )
        : CIMaterial( name )
    {
        m_type = eMaterialType::PHONG;

        ambient   = ambientColor;
        diffuse   = diffuseColor;
        specular  = specularColor;
        shininess = specularShininess;

        m_albedoMap   = albedoMap;
        m_specularMap = specularMap;
        m_normalMap   = normalMap;
    }


    CPhongMaterial::~CPhongMaterial()
    {
        // do nothing for now
    }

    void CPhongMaterial::bind( std::shared_ptr< CShader > shader )
    {
        shader->setVec3( "u_material.ambient", ambient );
        shader->setVec3( "u_material.diffuse", diffuse );
        shader->setVec3( "u_material.specular", specular );
        shader->setFloat( "u_material.shininess", shininess );

        if ( m_albedoMap )
        {
            shader->setInt( "u_albedoMap", 0 );
            shader->setInt( "u_albedoMapActive", 1 );
            glActiveTexture( GL_TEXTURE0 );
            m_albedoMap->bind();
        }
        else
        {
            shader->setInt( "u_albedoMapActive", 0 );
        }

        if ( m_specularMap )
        {
            shader->setInt( "u_specularMap", 1 );
            shader->setInt( "u_specularMapActive", 1 );
            glActiveTexture( GL_TEXTURE1 );
            m_specularMap->bind();
        }
        else
        {
            shader->setInt( "u_specularMapActive", 0 );
        }

        if ( m_normalMap )
        {
            shader->setInt( "u_normalMap", 2 );
            shader->setInt( "u_normalMapActive", 1 );
            glActiveTexture( GL_TEXTURE2 );
            m_normalMap->bind();
        }
        else
        {
            shader->setInt( "u_normalMapActive", 0 );
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