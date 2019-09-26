#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <lights/CILight.h>

namespace engine
{

    class CSpotLight : public CILight
    {

    public :

        CVec3   position;
        CVec3   direction;
        float32 atnConstant;
        float32 atnLinear;
        float32 atnQuadratic;
        float32 innerCutoff;
        float32 outerCutoff;

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