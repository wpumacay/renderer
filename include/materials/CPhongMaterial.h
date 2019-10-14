#pragma once

#include <materials/CIMaterial.h>

namespace engine
{

    class CPhongMaterial : public CIMaterial
    {

    public :

        CPhongMaterial( const std::string& name,
                        const CVec3& ambientColor,
                        const CVec3& diffuseColor,
                        const CVec3& specularColor,
                        float32 specularShininess,
                        CTexture* diffuseMap = nullptr,
                        CTexture* specularMap = nullptr,
                        CTexture* normalMap = nullptr );

        ~CPhongMaterial();

        void bind( CShader* shaderPtr ) override;
        void unbind() override;

        static eMaterialType GetStaticType() { return eMaterialType::PHONG; }

    protected :

        std::string _toStringInternal() override;

    };

}
