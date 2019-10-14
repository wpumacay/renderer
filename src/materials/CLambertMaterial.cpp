
#include <materials/CLambertMaterial.h>

namespace engine
{

    CLambertMaterial::CLambertMaterial( const std::string& name,
                                        const CVec3& ambientColor,
                                        const CVec3& diffuseColor,
                                        CTexture* albedoMap )
        : CIMaterial( name, 
                      ambientColor, diffuseColor, { 0.0f, 0.0f, 0.0f }, 1.0f, 
                      albedoMap, nullptr, nullptr )
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

        if ( m_albedoMap )
        {
            shaderPtr->setInt( "u_albedoMap", 0 );
            shaderPtr->setInt( "u_albedoMapActive", 1 );
            glActiveTexture( GL_TEXTURE0 ); // use slot-0 for albedo-map
            m_albedoMap->bind();
        }
        else
        {
            shaderPtr->setInt( "u_albedoMapActive", 0 );
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