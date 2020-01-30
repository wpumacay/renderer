#pragma once

#include <lights/CILight.h>

namespace engine
{

    class CPointLight : public CILight
    {

    public :

        CPointLight( const std::string& name,
                     const CVec3& ambientColor,
                     const CVec3& diffuseColor,
                     const CVec3& specularColor,
                     const CVec3& lightPosition,
                     float32 attenuationFactorConstant,
                     float32 attenuationFactorLinear,
                     float32 attenuationFactorQuadratic );

        ~CPointLight();

    protected :

        std::string _toStringInternal() const override;

    };

}