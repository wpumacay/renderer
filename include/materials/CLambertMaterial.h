#pragma once

#include <materials/CIMaterial.h>

namespace engine
{

    class CLambertMaterial : public CIMaterial
    {

    public :

        CLambertMaterial( const std::string& name,
                          const CVec3& ambientColor,
                          const CVec3& diffuseColor,
                          CTexture* diffuseMap = nullptr );

        ~CLambertMaterial();

        void bind( CShader* shaderPtr ) override;
        void unbind() override;

        static eMaterialType GetStaticType() { return eMaterialType::LAMBERT; }

    protected :

        std::string _toStringInternal() override;

    };

}
