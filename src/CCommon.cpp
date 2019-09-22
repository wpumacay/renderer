
#include <CCommon.h>
#include <utils/CLogger.h>

namespace engine
{

    std::vector< std::string > split( const std::string &txt, char separator )
    {
        std::vector< std::string > _res;

        int pos = txt.find( separator );
        if ( pos == std::string::npos )
        {
            _res.push_back( txt );
            return _res;
        }

        int initpos = 0;

        while ( pos != std::string::npos )
        {
            _res.push_back( txt.substr( initpos, pos - initpos ) );
            initpos = pos + 1;

            pos = txt.find( separator, initpos );
        }

        _res.push_back( txt.substr( initpos, std::min( pos, (int) txt.size() ) - initpos ) );

        return _res;
    }

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