#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <lights/CILight.h>

namespace engine
{

    class CPointLight : public CILight
    {

    public :

        CVec3   position;
        float32 atnConstant;
        float32 atnLinear;
        float32 atnQuadratic;

        CPointLight( const std::string& name,
                     const CVec3& ambientColor,
                     const CVec3& diffuseColor,
                     const CVec3& specularColor,
                     const CVec3& lightPosition,
                     float32 attenuationFactorConstant,
                     float32 attenuationFactorLinear,
                     float32 attenuationFactorQuadratic );

    protected :

        std::string _toStringInternal() override;

    };

}