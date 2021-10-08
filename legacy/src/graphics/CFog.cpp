
#include <graphics/CFog.h>

namespace engine
{

    std::string toString( const eFogType& type )
    {
        if ( type == eFogType::LINEAR ) return "linear";
        if ( type == eFogType::EXPONENTIAL ) return "exponential";

        ENGINE_CORE_ASSERT( false, "Invalid eFogType enum given" );

        return "undefined";
    }

    CFog::CFog( const eFogType& type,
                const CVec3& fogColor,
                float32 fogDensity,
                float32 fogGradient,
                float32 fogDistStart,
                float32 fogDistEnd )
    {
        m_type      = type;
        m_active    = true;

        color       = fogColor;
        density     = fogDensity;
        gradient    = fogGradient;
        distStart   = fogDistStart;
        distEnd     = fogDistEnd;
    }

    CFog::~CFog()
    {
        // nothing to release for now
    }

    void CFog::bind( CShader* shaderPtr )
    {
        shaderPtr->setInt( "u_fog.enabled", ( m_active ) ? 1 : 0 );
        shaderPtr->setInt( "u_fog.type", ( m_type == eFogType::LINEAR ) ? 0 : 1 );
        shaderPtr->setVec3( "u_fog.color", color );
        shaderPtr->setFloat( "u_fog.density", density );
        shaderPtr->setFloat( "u_fog.gradient", gradient );
        shaderPtr->setFloat( "u_fog.distStart", distStart );
        shaderPtr->setFloat( "u_fog.distEnd", distEnd );
    }

    void CFog::unbind()
    {
        // nothing to unbind for now
    }

    std::string CFog::toString() const
    {
        std::string _strRep;

        _strRep += "type        : " + engine::toString( m_type ) + "\n\r";
        _strRep += "color       : " + engine::toString( color ) + "\n\r";
        _strRep += "density     : " + std::to_string( density ) + "\n\r";
        _strRep += "gradient    : " + std::to_string( gradient ) + "\n\r";
        _strRep += "distStart   : " + std::to_string( distStart ) + "\n\r";
        _strRep += "distEnd     : " + std::to_string( distEnd ) + "\n\r";

        return _strRep;
    }

}
