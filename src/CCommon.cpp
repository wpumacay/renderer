
#include <CCommon.h>
#include <utils/CLogger.h>

namespace engine
{

    std::string toString( const eAxis& axis )
    {
        if ( axis == eAxis::X ) return "x";
        if ( axis == eAxis::Y ) return "y";
        if ( axis == eAxis::Z ) return "z";

        ENGINE_CORE_ASSERT( false, "Invalid eAxis type given." );

        return "undefined";
    }

}