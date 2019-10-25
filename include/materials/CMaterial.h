#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>
#include <shaders/CShader.h>
#include <core/CTexture.h>

namespace engine
{

    const int MATERIAL_ALBEDO_MAP_SLOT = 0;
    const int MATERIAL_SPECULAR_MAP_SLOT = 1;

    const CVec3 MATERIAL_DEFAULT_AMBIENT_COLOR = { 1.0f, 0.5f, 0.31f };
    const CVec3 MATERIAL_DEFAULT_DIFFUSE_COLOR = { 1.0f, 0.5f, 0.31f };
    const CVec3 MATERIAL_DEFAULT_SPECULAR_COLOR = { 1.0f, 0.5f, 0.31f };
    const float32 MATERIAL_DEFAULT_SHININESS = 32.0f;

    enum class eMaterialType
    {
        NONE = 0,
        LAMBERT, 
        PHONG,
        BLINN_PHONG
    };

    std::string toString( const eMaterialType& shading );

    class CMaterial
    {

    public :

        CVec3   ambient;
        CVec3   diffuse;
        CVec3   specular;
        float32 shininess;
        bool    transparent;
        float32 alpha;

        CMaterial( const std::string& name,
                   const eMaterialType& type,
                   const CVec3& ambientColor = MATERIAL_DEFAULT_AMBIENT_COLOR,
                   const CVec3& diffuseColor = MATERIAL_DEFAULT_DIFFUSE_COLOR,
                   const CVec3& specularColor = MATERIAL_DEFAULT_SPECULAR_COLOR,
                   float32 specularShininess = MATERIAL_DEFAULT_SHININESS,
                   CTexture* albedoMap = nullptr,
                   CTexture* specularMap = nullptr );

        ~CMaterial();

        void bind( CShader* shaderPtr );
        void unbind();

        std::string toString();

        void setType( const eMaterialType& type ) { m_type = type; }
        void setAlbedoMap( CTexture* albedoMap ) { m_albedoMap = albedoMap; }
        void setSpecularMap( CTexture* specularMap ) { m_specularMap = specularMap; }

        CTexture* albedoMap() const { return m_albedoMap; }
        CTexture* specularMap() const { return m_specularMap; }

        std::string name() const { return m_name; }
        eMaterialType type() const { return m_type; }

        static eMaterialType GetStaticType() { return eMaterialType::NONE; }

    protected :

        std::string     m_name;
        eMaterialType   m_type;
        CTexture*       m_albedoMap;
        CTexture*       m_specularMap;

    };

}