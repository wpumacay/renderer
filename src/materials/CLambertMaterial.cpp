
#include <materials/CLambertMaterial.h>

namespace engine
{

    CLambertMaterial::CLambertMaterial( const std::string& name,
                                        const CVec3& ambientColor,
                                        const CVec3& diffuseColor,
                                        std::shared_ptr< CTexture > diffuseMap )
        : CIMaterial( name )
    {
        ambient = ambientColor;
        diffuse = diffuseColor;

        m_diffuseMap = diffuseMap;
    }

    CLambertMaterial::~CLambertMaterial()
    {
        // do nothing for now
    }

    void CLambertMaterial::bind( CShader* shaderPtr )
    {
        shaderPtr->setVec3( "u_material.ambient", ambient );
        shaderPtr->setVec3( "u_material.diffuse", diffuse );

        if ( m_diffuseMap )
        {
            shaderPtr->setInt( "u_diffuseMap", 0 );
            shaderPtr->setInt( "u_diffuseMapActive", 1 );
            glActiveTexture( GL_TEXTURE0 );
            m_diffuseMap->bind();
        }
        else
        {
            shaderPtr->setInt( "u_diffuseMapActive", 0 );
        }
    }

    void CLambertMaterial::unbind()
    {
        if ( m_diffuseMap )
            m_diffuseMap->unbind();
    }

    std::string CLambertMaterial::_toStringInternal()
    {
        std::string _strRep;

        _strRep += "ambient     : " + engine::toString( ambient ) + "\n\r";
        _strRep += "diffuse     : " + engine::toString( diffuse ) + "\n\r";
        _strRep += "diffuseMap   : " + ( ( m_diffuseMap ) ? m_diffuseMap->name() : "none" ) + "\n\r";

        return _strRep;
    }

}