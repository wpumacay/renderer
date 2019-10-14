#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>
#include <shaders/CShader.h>
#include <core/CTexture.h>

namespace engine
{

    enum class eMaterialType
    {
        NONE = 0,
        LAMBERT, 
        PHONG,
        BLINN_PHONG // @TODO: not implemented yet
    };

    std::string toString( const eMaterialType& shading );

    class CIMaterial
    {

    public :

        CVec3   ambient;
        CVec3   diffuse;
        CVec3   specular;
        float32 shininess;
        float32 alpha;

        CIMaterial( const std::string& name,
                    const CVec3& ambientColor,
                    const CVec3& diffuseColor,
                    const CVec3& specularColor,
                    float32 specularShininess,
                    CTexture* diffuseMap,
                    CTexture* specularMap,
                    CTexture* normalMap );

        virtual ~CIMaterial();

        virtual void bind( CShader* shaderPtr ) = 0;
        virtual void unbind() = 0;

        std::string toString();

        void setDiffuseMap( CTexture* diffuseMap ) { m_diffuseMap = diffuseMap; }
        void setSpecularMap( CTexture* specularMap ) { m_specularMap = specularMap; }
        void setNormalMap( CTexture* normalMap ) { m_normalMap = normalMap; }

        CTexture* diffuseMap() const { return m_diffuseMap; }
        CTexture* specularMap() const { return m_specularMap; }
        CTexture* normalMap() const { return m_normalMap; }

        std::string name() const { return m_name; }
        eMaterialType type() const { return m_type; }

        static eMaterialType GetStaticType() { return eMaterialType::NONE; }

    protected :

        virtual std::string _toStringInternal() = 0;

        std::string     m_name;
        eMaterialType   m_type;
        CTexture*       m_diffuseMap;
        CTexture*       m_specularMap;
        CTexture*       m_normalMap;

    };

}