
#include <materials/CIMaterial.h>

namespace engine
{

    std::string toString( const eMaterialType& type )
    {
        if ( type == eMaterialType::LAMBERT ) return "lambert";
        if ( type == eMaterialType::PHONG ) return "phong";

        ENGINE_CORE_ASSERT( false, "Invalid eMaterialType enum given" );

        return "undefined";
    }
    CIMaterial::CIMaterial( const std::string& name )
    {
        m_name = name;
        m_type = eMaterialType::NONE;
    }

    CIMaterial::~CIMaterial()
    {
        // do nothing for now
    }


    std::string CIMaterial::toString()
    {
        std::string _strRep;

        _strRep += "name    : " + m_name + "\n\r";
        _strRep += "type    : " + engine::toString( m_type ) + "\n\r";
        _strRep += _toStringInternal();

        return _strRep;
    }

}