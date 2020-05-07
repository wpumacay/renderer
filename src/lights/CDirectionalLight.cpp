
#include <lights/CDirectionalLight.h>

namespace engine
{

    CDirectionalLight::CDirectionalLight( const std::string& name,
                                          const CVec3& ambientColor,
                                          const CVec3& diffuseColor,
                                          const CVec3& specularColor,
                                          const CVec3& lightDirection )
        : CILight( name, ambientColor, diffuseColor, specularColor )
    {
        m_type      = eLightType::DIRECTIONAL;
        direction   = lightDirection;

    #ifdef ENGINE_TRACK_ALLOCS
        if ( tinyutils::Logger::IsActive() )
            ENGINE_CORE_TRACE( "Allocs: Created DirectionalLight @ {0}", tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Allocs: Created DirectionalLight @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    CDirectionalLight::~CDirectionalLight()
    {
    #ifdef ENGINE_TRACK_ALLOCS
        if ( tinyutils::Logger::IsActive() )
            ENGINE_CORE_TRACE( "Allocs: Destroyed DirectionalLight @ {0}", tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Allocs: Destroyed DirectionalLight @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    std::string CDirectionalLight::_toStringInternal() const
    {
        std::string _strRep;

        _strRep += "direction   : " + engine::toString( direction ) + "\n\r";

        return _strRep;
    }

}