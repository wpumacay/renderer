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
                          std::shared_ptr< CTexture > albedoMap = nullptr );

        ~CLambertMaterial();

        void setAlbedoMap( std::shared_ptr< CTexture > albedoMap ) { m_albedoMap = albedoMap; }

        void bind( std::shared_ptr< CShader > shader ) override;
        void unbind() override;

        std::shared_ptr< CTexture > albedoMap() const { return m_albedoMap; }

    protected :

        std::string _toStringInternal() override;

        std::shared_ptr< CTexture > m_albedoMap;

    };

}
