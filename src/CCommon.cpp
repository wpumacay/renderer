
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

    std::string getFilenameFromFilePath(  const std::string& filepath )
    {
        return engine::split( filepath, '/' ).back();
    }

    std::string getFoldernameFromFilePath(  const std::string& filepath )
    {
        auto _pathParts = engine::split( filepath, '/' );
        if ( _pathParts.size() < 2 )
        {
            return "./";
        }

        return _pathParts[ _pathParts.size() - 2 ];
    }

    std::string getFolderpathFromFilePath( const std::string& filepath )
    {
        auto _pathParts = engine::split( filepath, '/' );
        if ( _pathParts.size() < 2 )
        {
            return "./";
        }

        std::stringstream _ss;
        for ( size_t i = 0; i < _pathParts.size() - 1; i++ )
            _ss << _pathParts[i] << "/";

        return _ss.str();
    }

    std::string getFilenameNoExtensionFromFilePath(  const std::string& filepath )
    {
        return engine::split( engine::split( filepath, '/' ).back(), '.' ).front();
    }

    std::string toString( const eAxis& axis )
    {
        if ( axis == eAxis::X ) return "x";
        if ( axis == eAxis::Y ) return "y";
        if ( axis == eAxis::Z ) return "z";

        ENGINE_CORE_ASSERT( false, "Invalid eAxis enum given." );

        return "undefined";
    }

    std::string toString( const ePixelDataType& dtype )
    {
        if ( dtype == ePixelDataType::UINT_8 ) return "uint_8";
        if ( dtype == ePixelDataType::UINT_32 ) return "uint_32";
        if ( dtype == ePixelDataType::UINT_24_8 ) return "uint_24_8";
        if ( dtype == ePixelDataType::FLOAT_32 ) return "float_32";

        ENGINE_CORE_ASSERT( false, "Invalid ePixelDataType given" );

        return 0;
    }

    uint32 toOpenGLEnum( const ePixelDataType& dtype )
    {
        if ( dtype == ePixelDataType::UINT_8 ) return GL_UNSIGNED_BYTE;
        if ( dtype == ePixelDataType::UINT_32 ) return GL_UNSIGNED_INT;
        if ( dtype == ePixelDataType::UINT_24_8 ) return GL_UNSIGNED_INT_24_8;
        if ( dtype == ePixelDataType::FLOAT_32 ) return GL_FLOAT;

        ENGINE_CORE_ASSERT( false, "Invalid ePixelDataType given" );

        return 0;
    }

}