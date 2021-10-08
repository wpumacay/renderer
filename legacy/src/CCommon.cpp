
#include <CCommon.h>

namespace engine
{
    std::string ToString( const eAxis& axis )
    {
        /**/ if ( axis == eAxis::X ) return "x";
        else if ( axis == eAxis::Y ) return "y";
        else if ( axis == eAxis::Z ) return "z";

        LOG_CORE_CRITICAL( "Invalid eAxis enum given." );
        return "undefined";
    }

    std::string ToString( const ePixelDataType& dtype )
    {
        /**/ if ( dtype == ePixelDataType::UINT_8 ) return "uint_8";
        else if ( dtype == ePixelDataType::UINT_32 ) return "uint_32";
        else if ( dtype == ePixelDataType::UINT_24_8 ) return "uint_24_8";
        else if ( dtype == ePixelDataType::FLOAT_32 ) return "float_32";

        LOG_CORE_CRITICAL( "Invalid ePixelDataType given" );
        return 0;
    }

    uint32 ToOpenGLEnum( const ePixelDataType& dtype )
    {
        /**/ if ( dtype == ePixelDataType::UINT_8 ) return GL_UNSIGNED_BYTE;
        else if ( dtype == ePixelDataType::UINT_32 ) return GL_UNSIGNED_INT;
        else if ( dtype == ePixelDataType::UINT_24_8 ) return GL_UNSIGNED_INT_24_8;
        else if ( dtype == ePixelDataType::FLOAT_32 ) return GL_FLOAT;

        LOG_CORE_CRITICAL( "Invalid ePixelDataType given" );
        return 0;
    }

    std::string ToString( const eBufferUsage& usage )
    {
        /**/ if ( usage == eBufferUsage::STATIC ) return "static";
        else if ( usage == eBufferUsage::DYNAMIC ) return "dynamic";

        ENGINE_CORE_CRITICAL( "ToString >>> Invalid eBufferUsage enum given" );
        return "undefined";
    }

    uint32 ToOpenGLEnum( const eBufferUsage& usage )
    {
        /**/ if ( usage == eBufferUsage::STATIC ) return GL_STATIC_DRAW;
        else if ( usage == eBufferUsage::DYNAMIC ) return GL_DYNAMIC_DRAW;

        ENGINE_CORE_CRITICAL( "ToOpenGLEnum >>> Invalid eBufferUsage enum given" );
        return 0;
    }
}
