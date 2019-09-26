
#include <lights/CDirectionalLight.h>

namespace engine
{

    CDirectionalLight::CDirectionalLight( const std::string& name,
                                          const CVec3& ambientColor,
                                          const CVec3& diffuseColor,
                                          const CVec3& specularColor,
                                          const CVec3& lightDirection )
        : CILight( name, ambientColor, diffuseColor, specularColor )
    {
        m_type      = eLightType::DIRECTIONAL;
        direction   = lightDirection;
    }

    std::string CDirectionalLight::_toStringInternal()
    {
        std::string _strRep;

        _strRep += "direction   : " + engine::toString( direction ) + "\n\r";

        return _strRep;
    }

}