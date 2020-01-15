#pragma once

#include <lights/CILight.h>

namespace engine
{

    class CSpotLight : public CILight
    {

    public :

        CSpotLight( const std::string& name,
                    const CVec3& ambientColor,
                    const CVec3& diffuseColor,
                    const CVec3& specularColor,
                    const CVec3& lightPosition,
                    const CVec3& lightDirection,
                    float32 attenuationFactorConstant,
                    float32 attenuationFactorLinear,
                    float32 attenuationFactorQuadratic,
                    float32 innerCutoff,
                    float32 outerCUtoff );

    protected :

        std::string _toStringInternal() override;

    };

}