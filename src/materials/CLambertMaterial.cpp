
#include <materials/CLambertMaterial.h>

namespace engine
{

    CLambertMaterial::CLambertMaterial( const std::string& name,
                                        const CVec3& ambientColor,
                                        const CVec3& diffuseColor,
                                        CTexture* diffuseMap )
        : CIMaterial( name, 
                      ambientColor, diffuseColor, { 0.0f, 0.0f, 0.0f }, 1.0f, 
                      diffuseMap, nullptr, nullptr )
    {
        m_type = eMaterialType::LAMBERT;
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