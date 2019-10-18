
#include <materials/CMaterial.h>

namespace engine
{

    std::string toString( const eMaterialType& type )
    {
        if ( type == eMaterialType::LAMBERT ) return "lambert";
        if ( type == eMaterialType::PHONG ) return "phong";
        if ( type == eMaterialType::BLINN_PHONG ) return "blinn_phong";

        ENGINE_CORE_ASSERT( false, "Invalid eMaterialType enum given" );

        return "undefined";
    }

    CMaterial::CMaterial( const std::string& name,
                          const eMaterialType& type,
                          const CVec3& ambientColor,
                          const CVec3& diffuseColor,
                          const CVec3& specularColor,
                          float32 specularShininess,
                          CTexture* albedoMap,
                          CTexture* specularMap,
                          CTexture* normalMap )
    {
        m_name  = name;
        m_type  = type;

        ambient     = ambientColor;
        diffuse     = diffuseColor;
        specular    = specularColor;
        shininess   = specularShininess;
        transparent = false;
        alpha       = 1.0f;

        m_albedoMap     = albedoMap;
        m_specularMap   = specularMap;
        m_normalMap     = normalMap;
    }

    CMaterial::~CMaterial()
    {
        m_albedoMap = nullptr;
        m_specularMap = nullptr;
        m_normalMap = nullptr;
    }

    void CMaterial::bind( engine::CShader* shaderPtr )
    {
        if ( m_type == eMaterialType::LAMBERT )
        {
            shaderPtr->setVec3( "u_material.ambient", ambient );
            shaderPtr->setVec3( "u_material.diffuse", diffuse );
    
            if ( m_albedoMap )
            {
                shaderPtr->setInt( "u_albedoMap", MATERIAL_ALBEDO_MAP_SLOT );
                shaderPtr->setInt( "u_albedoMapActive", 1 );
                glActiveTexture( GL_TEXTURE0 + MATERIAL_ALBEDO_MAP_SLOT );
                m_albedoMap->bind();
            }
            else
            {
                shaderPtr->setInt( "u_albedoMapActive", 0 );
            }
        }
        else if ( m_type == eMaterialType::PHONG ||
                  m_type == eMaterialType::BLINN_PHONG )
        {
            shaderPtr->setVec3( "u_material.ambient", ambient );
            shaderPtr->setVec3( "u_material.diffuse", diffuse );
            shaderPtr->setVec3( "u_material.specular", specular );
            shaderPtr->setFloat( "u_material.shininess", shininess );

            if ( m_albedoMap )
            {
                shaderPtr->setInt( "u_material.albedoMap", MATERIAL_ALBEDO_MAP_SLOT );
                shaderPtr->setInt( "u_material.albedoMapActive", 1 );
                glActiveTexture( GL_TEXTURE0 + MATERIAL_ALBEDO_MAP_SLOT );
                m_albedoMap->bind();
            }
            else
            {
                shaderPtr->setInt( "u_material.albedoMapActive", 0 );
            }

            if ( m_specularMap )
            {
                shaderPtr->setInt( "u_material.specularMap", MATERIAL_SPECULAR_MAP_SLOT );
                shaderPtr->setInt( "u_material.specularMapActive", 1 );
                glActiveTexture( GL_TEXTURE0 + MATERIAL_SPECULAR_MAP_SLOT );
                m_specularMap->bind();
            }
            else
            {
                shaderPtr->setInt( "u_material.specularMapActive", 0 );
            }

            if ( m_normalMap )
            {
                shaderPtr->setInt( "u_material.normalMap", MATERIAL_NORMAL_MAP_SLOT );
                shaderPtr->setInt( "u_material.normalMapActive", 1 );
                glActiveTexture( GL_TEXTURE0 + MATERIAL_NORMAL_MAP_SLOT );
                m_normalMap->bind();
            }
            else
            {
                shaderPtr->setInt( "u_material.normalMapActive", 0 );
            }
        }
    }

    void CMaterial::unbind()
    {
        // unbind albedo-map, specular-map and normal-map (end with texture-0 to go back to 
        // default-texture 0 being active, in case someone binds this slot without activating it)
        glActiveTexture( GL_TEXTURE2 );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glActiveTexture( GL_TEXTURE1 );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glActiveTexture( GL_TEXTURE0 );
        glBindTexture( GL_TEXTURE_2D, 0 );
    }

    std::string CMaterial::toString()
    {
        std::string _strRep;

        _strRep += "name    : " + m_name + "\n\r";
        _strRep += "type    : " + engine::toString( m_type ) + "\n\r";
        
        if ( m_type == eMaterialType::LAMBERT )
        {
            _strRep += "ambient     : " + engine::toString( ambient ) + "\n\r";
            _strRep += "diffuse     : " + engine::toString( diffuse ) + "\n\r";
            _strRep += "albedoMap   : " + ( ( m_albedoMap ) ? m_albedoMap->name() : "none" ) + "\n\r";
        }
        else if ( m_type == eMaterialType::PHONG ||
                  m_type == eMaterialType::BLINN_PHONG )
        {
            _strRep += "ambient     : " + engine::toString( ambient ) + "\n\r";
            _strRep += "diffuse     : " + engine::toString( diffuse ) + "\n\r";
            _strRep += "specular    : " + engine::toString( specular ) + "\n\r";
            _strRep += "shininess   : " + std::to_string( shininess ) + "\n\r";
            _strRep += "albedoMap  : " + ( ( m_albedoMap ) ? m_albedoMap->name() : "none" ) + "\n\r";
            _strRep += "specularMap : " + ( ( m_specularMap ) ? m_specularMap->name() : "none" ) + "\n\r";
            _strRep += "normalMap   : " + ( ( m_normalMap ) ? m_normalMap->name() : "none" ) + "\n\r";
        }
 
        return _strRep;
    }

}