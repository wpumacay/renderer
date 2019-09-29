
#include <materials/CLambertMaterial.h>

namespace engine
{

    CLambertMaterial::CLambertMaterial( const std::string& name,
                                        const CVec3& ambientColor,
                                        const CVec3& diffuseColor,
                                        std::shared_ptr< CTexture > albedoMap )
        : CIMaterial( name )
    {
        ambient = ambientColor;
        diffuse = diffuseColor;

        m_albedoMap = albedoMap;
    }

    CLambertMaterial::~CLambertMaterial()
    {
        // do nothing for now
    }

    void CLambertMaterial::bind( std::shared_ptr< CShader > shader )
    {
        shader->setVec3( "u_material.ambient", ambient );
        shader->setVec3( "u_material.diffuse", diffuse );

        if ( m_albedoMap )
        {
            shader->setInt( "u_albedoMap", 0 );
            shader->setInt( "u_albedoMapActive", 1 );
            glActiveTexture( GL_TEXTURE0 );
            m_albedoMap->bind();
        }
        else
        {
            shader->setInt( "u_albedoMapActive", 0 );
        }
    }

    void CLambertMaterial::unbind()
    {
        if ( m_albedoMap )
            m_albedoMap->unbind();
    }

    std::string CLambertMaterial::_toStringInternal()
    {
        std::string _strRep;

        _strRep += "ambient     : " + engine::toString( ambient ) + "\n\r";
        _strRep += "diffuse     : " + engine::toString( diffuse ) + "\n\r";
        _strRep += "albedoMap   : " + ( ( m_albedoMap ) ? m_albedoMap->name() : "none" ) + "\n\r";

        return _strRep;
    }

}