
#include <materials/CPhongMaterial.h>

namespace engine
{

    CPhongMaterial::CPhongMaterial( const std::string& name,
                                    const CVec3& ambientColor,
                                    const CVec3& diffuseColor,
                                    const CVec3& specularColor,
                                    float32 specularShininess,
                                    std::shared_ptr< CTexture > diffuseMap,
                                    std::shared_ptr< CTexture > specularMap,
                                    std::shared_ptr< CTexture > normalMap )
        : CIMaterial( name )
    {
        m_type = eMaterialType::PHONG;

        ambient   = ambientColor;
        diffuse   = diffuseColor;
        specular  = specularColor;
        shininess = specularShininess;

        m_diffuseMap  = diffuseMap;
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

        if ( m_diffuseMap )
        {
            shader->setInt( "u_material.diffuseMap", 0 );
            shader->setInt( "u_material.diffuseMapActive", 1 );
            glActiveTexture( GL_TEXTURE0 );
            m_diffuseMap->bind();
        }
        else
        {
            shader->setInt( "u_material.diffuseMapActive", 0 );
        }

        if ( m_specularMap )
        {
            shader->setInt( "u_material.specularMap", 1 );
            shader->setInt( "u_material.specularMapActive", 1 );
            glActiveTexture( GL_TEXTURE1 );
            m_specularMap->bind();
        }
        else
        {
            shader->setInt( "u_material.specularMapActive", 0 );
        }

        if ( m_normalMap )
        {
            shader->setInt( "u_material.normalMap", 2 );
            shader->setInt( "u_material.normalMapActive", 1 );
            glActiveTexture( GL_TEXTURE2 );
            m_normalMap->bind();
        }
        else
        {
            shader->setInt( "u_material.normalMapActive", 0 );
        }
    }

    void CPhongMaterial::unbind()
    {
        if ( m_diffuseMap )
            m_diffuseMap->unbind();

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
        _strRep += "diffuseMap  : " + ( ( m_diffuseMap ) ? m_diffuseMap->name() : "none" ) + "\n\r";
        _strRep += "specularMap : " + ( ( m_specularMap ) ? m_specularMap->name() : "none" ) + "\n\r";
        _strRep += "normalMap   : " + ( ( m_normalMap ) ? m_normalMap->name() : "none" ) + "\n\r";

        return _strRep;
    }

}