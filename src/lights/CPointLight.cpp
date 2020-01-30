
#include <lights/CPointLight.h>

namespace engine
{

    CPointLight::CPointLight( const std::string& name,
                              const CVec3& ambientColor,
                              const CVec3& diffuseColor,
                              const CVec3& specularColor,
                              const CVec3& lightPosition,
                              float32 attenuationFactorConstant,
                              float32 attenuationFactorLinear,
                              float32 attenuationFactorQuadratic )
        : CILight( name, ambientColor, diffuseColor, specularColor )
    {
        m_type          = eLightType::POINT;
        position        = lightPosition;
        atnConstant     = attenuationFactorConstant;
        atnLinear       = attenuationFactorLinear;
        atnQuadratic    = attenuationFactorQuadratic;

    #ifdef ENGINE_TRACK_ALLOCS
        if ( CLogger::IsActive() )
            ENGINE_CORE_TRACE( "Allocs: Created PointLight @ {0}", engine::pointerToHexAddress( this ) );
        else
            std::cout << "Allocs: Created PointLight @ " << engine::pointerToHexAddress( this ) << std::endl;
    #endif
    }

    CPointLight::~CPointLight()
    {
    #ifdef ENGINE_TRACK_ALLOCS
        if ( CLogger::IsActive() )
            ENGINE_CORE_TRACE( "Allocs: Destroyed PointLight @ {0}", engine::pointerToHexAddress( this ) );
        else
            std::cout << "Allocs: Destroyed PointLight @ " << engine::pointerToHexAddress( this ) << std::endl;
    #endif
    }

    std::string CPointLight::_toStringInternal() const
    {
        std::string _strRep;

        _strRep += "position    : " + engine::toString( position ) + "\n\r";
        _strRep += "atnConstant : " + std::to_string( atnConstant ) + "\n\r";
        _strRep += "atnLinear   : " + std::to_string( atnLinear ) + "\n\r";
        _strRep += "atnQuadratic: " + std::to_string( atnQuadratic ) + "\n\r";

        return _strRep;
    }
}