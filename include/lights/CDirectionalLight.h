#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <lights/CILight.h>

namespace engine
{

    class CDirectionalLight : public CILight
    {

    public :

        CVec3 direction;

        CDirectionalLight( const std::string& name,
                           const CVec3& ambientColor,
                           const CVec3& diffuseColor,
                           const CVec3& specularColor,
                           const CVec3& lightDirection );

    protected :

        std::string _toStringInternal() override;

    };

}