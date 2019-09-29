#pragma once

#include <materials/CIMaterial.h>
#include <core/CTexture.h>

namespace engine
{

    class CLambertMaterial : public CIMaterial
    {

    public :

        CVec3 ambient;
        CVec3 diffuse;

        CLambertMaterial( const std::string& name,
                          const CVec3& ambientColor,
                          const CVec3& diffuseColor,
                          std::shared_ptr< CTexture > diffuseMap = nullptr );

        ~CLambertMaterial();

        void setDiffuseMap( std::shared_ptr< CTexture > diffuseMap ) { m_diffuseMap = diffuseMap; }

        void bind( std::shared_ptr< CShader > shader ) override;
        void unbind() override;

        std::shared_ptr< CTexture > diffuseMap() const { return m_diffuseMap; }

    protected :

        std::string _toStringInternal() override;

        std::shared_ptr< CTexture > m_diffuseMap;

    };

}
