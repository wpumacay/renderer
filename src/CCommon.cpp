
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

    std::string toString( const ePixelFormat& format )
    {
        if ( format == ePixelFormat::RGB ) return "rgb";
        if ( format == ePixelFormat::RGBA ) return "rgba";

        ENGINE_CORE_ASSERT( false, "Invalid ePixelFormat type given" );

        return "undefined";
    }

    uint32 toOpenGLEnum( const ePixelFormat& format )
    {
        if ( format == ePixelFormat::RGB ) return GL_RGB;
        if ( format == ePixelFormat::RGBA ) return GL_RGBA;

        ENGINE_CORE_ASSERT( false, "Invalid ePixelFormat type given" );

        return 0;
    }

}