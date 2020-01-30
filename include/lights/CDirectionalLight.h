#pragma once

#include <lights/CILight.h>

namespace engine
{

    class CDirectionalLight : public CILight
    {

    public :

        CDirectionalLight( const std::string& name,
                           const CVec3& ambientColor,
                           const CVec3& diffuseColor,
                           const CVec3& specularColor,
                           const CVec3& lightDirection );
        ~CDirectionalLight();

    protected :

        std::string _toStringInternal() const override;

    };

}