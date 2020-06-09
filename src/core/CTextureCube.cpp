
#include <core/CTextureCube.h>

namespace engine
{

    /**************************************************************************
    *                           CTextureCubeData impl.                        *
    ***************************************************************************/

    CTextureCubeData::CTextureCubeData()
    {
        name        = "undefined";
        sidesData   = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
        width       = 0;
        height      = 0;
        channels    = 0;
        format      = eTextureFormat::NONE;

    #if defined(ENGINE_TRACK_ALLOCS)
        if ( tinyutils::Logger::IsActive() )
            ENGINE_CORE_TRACE( "Allocs: Created TextureCubeData @ {0}", tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Allocs: Created TextureCubeData @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    CTextureCubeData::~CTextureCubeData()
    {
        for ( size_t i = 0; i < sidesData.size(); i++ )
            delete sidesData[i];

    #if defined(ENGINE_TRACK_ALLOCS)
        if ( tinyutils::Logger::IsActive() )
            ENGINE_CORE_TRACE( "Allocs: Destroyed TextureCubeData @ {0}", tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Allocs: Destroyed TextureCubeData @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    std::string toString( const CTextureCubeData& texCubeData )
    {
        std::string _strRep;

        _strRep += "name    : " + texCubeData.name + "\n\r";
        _strRep += "width   : " + std::to_string( texCubeData.width ) + "\n\r";
        _strRep += "height  : " + std::to_string( texCubeData.height ) + "\n\r";
        _strRep += "chnls   : " + std::to_string( texCubeData.channels ) + "\n\r";
        _strRep += "format  : " + engine::toString( texCubeData.format ) + "\n\r";

        return _strRep;
    }

    /**************************************************************************
    *                              CCubeTexture impl.                         *
    ***************************************************************************/

    CTextureCube::CTextureCube( std::unique_ptr<CTextureCubeData> texCubeData )
    {
        m_data = std::move( texCubeData );
        m_openglId = 0;

        /* create gl-texture resource with the given information ****************/
        glGenTextures( 1, &m_openglId );
        glBindTexture( GL_TEXTURE_CUBE_MAP, m_openglId );

        /* configure wrapping mode */
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

        /* configure min-mag filtering mode */
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        for ( size_t i = 0; i < 6; i++ )
        {
            glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                          toOpenGLEnum( m_data->format ), m_data->width, m_data->height, 0, 
                          toOpenGLEnum( m_data->format ), GL_UNSIGNED_BYTE, m_data->sidesData[i] );
        }

        glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
        /***********************************************************************/

    #if defined(ENGINE_TRACK_ALLOCS)
        if ( tinyutils::Logger::IsActive() )
            ENGINE_CORE_TRACE( "Allocs: Created TextureCube @ {0}", tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Allocs: Created TextureCube @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    CTextureCube::~CTextureCube()
    {
        glDeleteTextures( 1, &m_openglId );
        m_data = nullptr;

    #if defined(ENGINE_TRACK_ALLOCS)
        if ( tinyutils::Logger::IsActive() )
            ENGINE_CORE_TRACE( "Allocs: Destroyed TextureCube @ {0}", tinyutils::PointerToHexAddress( this ) );
        else
            std::cout << "Allocs: Destroyed TextureCube @ " << tinyutils::PointerToHexAddress( this ) << std::endl;
    #endif
    }

    void CTextureCube::bind()
    {
        glBindTexture( GL_TEXTURE_CUBE_MAP, m_openglId );
    }

    void CTextureCube::unbind()
    {
        glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
    }

}