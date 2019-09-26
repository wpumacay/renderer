
#include <lights/CSpotLight.h>

namespace engine
{

    CSpotLight::CSpotLight( const std::string& name,
                            const CVec3& ambientColor,
                            const CVec3& diffuseColor,
                            const CVec3& specularColor,
                            const CVec3& lightPosition,
                            const CVec3& lightDirection,
                            float32 attenuationFactorConstant,
                            float32 attenuationFactorLinear,
                            float32 attenuationFactorQuadratic,
                            float32 lightInnerCutoff,
                            float32 lightOuterCutoff )
        : CILight( name,ambientColor, diffuseColor, specularColor )
    {
        m_type          = eLightType::SPOT;
        position        = lightPosition;
        direction       = lightDirection;
        atnConstant     = attenuationFactorConstant;
        atnLinear       = attenuationFactorLinear;
        atnQuadratic    = attenuationFactorQuadratic;
        innerCutoff     = lightInnerCutoff;
        outerCutoff     = lightOuterCutoff;
    }

    std::string CSpotLight::_toStringInternal()
    {
        std::string _strRep;

        _strRep += "position    : " + engine::toString( position ) + "\n\r";
        _strRep += "direction   : " + engine::toString( direction ) + "\n\r";
        _strRep += "atnConstant : " + std::to_string( atnConstant ) + "\n\r";
        _strRep += "atnLinear   : " + std::to_string( atnLinear ) + "\n\r";
        _strRep += "atnQuadratic: " + std::to_string( atnQuadratic ) + "\n\r";
        _strRep += "innerCutoff : " + std::to_string( innerCutoff ) + "\n\r";
        _strRep += "outerCutoff : " + std::to_string( outerCutoff ) + "\n\r";

        return _strRep;
    }

}