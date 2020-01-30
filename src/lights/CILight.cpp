
#include <lights/CILight.h>

namespace engine
{

    std::string toString( const eLightType& type )
    {
        if ( type == eLightType::DIRECTIONAL ) return "directional";
        if ( type == eLightType::POINT ) return "point";
        if ( type == eLightType::SPOT ) return "spot";

        ENGINE_CORE_ASSERT( false, "Invalid eLightType enum given" );

        return "undefined";
    }

    CILight::CILight( const std::string& name,
                      const CVec3& ambientColor,
                      const CVec3& diffuseColor,
                      const CVec3& specularColor )
    {
        m_name      = name;
        m_type      = eLightType::NONE;
        ambient     = ambientColor;
        diffuse     = diffuseColor;
        specular    = specularColor;
        intensity   = 1.0f;
        enabled     = true;

        direction       = { 0.0f, 0.0f, -1.0f };
        position        = { 0.0f, 0.0f, 1.0f };
        atnConstant     = 1.0f;
        atnLinear       = 0.0f;
        atnQuadratic    = 0.0f;
        innerCutoff     = 0.0f;
        outerCutoff     = 0.0f;
    }

    CILight::~CILight()
    {
        // nothing to release for now
    }

    std::string CILight::toString() const
    {
        std::string _strRep;

        _strRep += "name        : " + m_name + "\n\r";
        _strRep += "type        : " + engine::toString( m_type ) + "\n\r";
        _strRep += "cAmbient    : " + engine::toString( ambient ) + "\n\r";
        _strRep += "cDiffuse    : " + engine::toString( diffuse ) + "\n\r";
        _strRep += "cSpecular   : " + engine::toString( specular ) + "\n\r";
        _strRep += "intensiry   : " + std::to_string( intensity ) + "\n\r";
        _strRep += "enabled     : " + std::string( ( enabled ) ? "true" : "false" ) + "\n\r";

        _strRep += _toStringInternal();

        return _strRep;
    }
}