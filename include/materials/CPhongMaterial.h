#pragma once

#include <materials/CIMaterial.h>
#include <core/CTexture.h>

namespace engine
{

    class CPhongMaterial : public CIMaterial
    {

    public :

        CVec3   ambient;
        CVec3   diffuse;
        CVec3   specular;
        float32 shininess;

        CPhongMaterial( const std::string& name,
                        const CVec3& ambientColor,
                        const CVec3& diffuseColor,
                        const CVec3& specularColor,
                        float32 shininess,
                        CTexture* diffuseMap = nullptr,
                        CTexture* specularMap = nullptr,
                        CTexture* normalMap = nullptr );

        ~CPhongMaterial();

        void setDiffuseMap( CTexture* diffuseMap ) { m_diffuseMap = diffuseMap; }
        void setSpecularMap( CTexture* specularMap ) { m_specularMap = specularMap; }
        void setNormalMap( CTexture* normalMap ) { m_normalMap = normalMap; }

        void bind( CShader* shaderPtr ) override;
        void unbind() override;

        CTexture* diffuseMap() const { return m_diffuseMap; }
        CTexture* specularMap() const { return m_specularMap; }
        CTexture* normalMap() const { return m_normalMap; }

    protected :

        std::string _toStringInternal() override;

    private :

        CTexture* m_diffuseMap;
        CTexture* m_specularMap;
        CTexture* m_normalMap;
    };

}
