
#include <CCommon.h>

namespace engine
{
    std::string toString( const eAxis& axis )
    {
        if ( axis == eAxis::X ) return "x";
        if ( axis == eAxis::Y ) return "y";
        if ( axis == eAxis::Z ) return "z";

        LOG_CORE_CRITICAL( "Invalid eAxis enum given." );
        return "undefined";
    }

    std::string toString( const ePixelDataType& dtype )
    {
        if ( dtype == ePixelDataType::UINT_8 ) return "uint_8";
        if ( dtype == ePixelDataType::UINT_32 ) return "uint_32";
        if ( dtype == ePixelDataType::UINT_24_8 ) return "uint_24_8";
        if ( dtype == ePixelDataType::FLOAT_32 ) return "float_32";

        LOG_CORE_CRITICAL( "Invalid ePixelDataType given" );
        return 0;
    }

    uint32 toOpenGLEnum( const ePixelDataType& dtype )
    {
        if ( dtype == ePixelDataType::UINT_8 ) return GL_UNSIGNED_BYTE;
        if ( dtype == ePixelDataType::UINT_32 ) return GL_UNSIGNED_INT;
        if ( dtype == ePixelDataType::UINT_24_8 ) return GL_UNSIGNED_INT_24_8;
        if ( dtype == ePixelDataType::FLOAT_32 ) return GL_FLOAT;

        LOG_CORE_CRITICAL( "Invalid ePixelDataType given" );
        return 0;
    }

}