
#include <materials/CIMaterial.h>

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
    CIMaterial::CIMaterial( const std::string& name,
                            const CVec3& ambientColor,
                            const CVec3& diffuseColor,
                            const CVec3& specularColor,
                            float32 specularShininess,
                            CTexture* diffuseMap,
                            CTexture* specularMap,
                            CTexture* normalMap )
    {
        m_name  = name;
        m_type  = eMaterialType::NONE;

        ambient     = ambientColor;
        diffuse     = diffuseColor;
        specular    = specularColor;
        shininess   = specularShininess;

        m_diffuseMap    = diffuseMap;
        m_specularMap   = specularMap;
        m_normalMap     = normalMap;
    }

    CIMaterial::~CIMaterial()
    {
        // do nothing for now
    }

    std::string CIMaterial::toString()
    {
        std::string _strRep;

        _strRep += "name    : " + m_name + "\n\r";
        _strRep += "type    : " + engine::toString( m_type ) + "\n\r";
        _strRep += _toStringInternal();

        return _strRep;
    }

}