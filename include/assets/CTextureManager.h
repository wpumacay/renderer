#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <core/CTexture.h>
#include <core/CTextureCube.h>

#include <dirent.h>

namespace engine
{

    class CTextureManager
    {

    public :

        static void Init();
        static void Release();

        ~CTextureManager();

        static std::shared_ptr< CTextureData > LoadTextureData( const std::string& filepath );
        static std::shared_ptr< CTexture > LoadTexture( const std::string& filepath,
                                                        const eTextureFilter& filterMin = eTextureFilter::NEAREST,
                                                        const eTextureFilter& filterMag = eTextureFilter::NEAREST,
                                                        const eTextureWrap& wrapU = eTextureWrap::REPEAT,
                                                        const eTextureWrap& wrapV = eTextureWrap::REPEAT,
                                                        const CVec4& borderColorU = { 0.0f, 0.0f, 0.0f, 1.0f },
                                                        const CVec4& borderColorV = { 0.0f, 0.0f, 0.0f, 1.0f },
                                                        const ePixelDataType& dtype = ePixelDataType::UINT_8,
                                                        uint32 textureUnit = 0 );
        static std::shared_ptr< CTexture > LoadTexture( const std::string& filepath,
                                                        const CTextureOptions& texOptions );

        static std::shared_ptr< CTextureCubeData > LoadTextureCubeData( const std::array< std::string, 6 >& filepaths );
        static std::shared_ptr< CTextureCube > LoadTextureCube( const std::array< std::string, 6 >& filepaths );

        static std::shared_ptr< CTextureData > GetCachedTextureData( const std::string& texDataId );
        static std::shared_ptr< CTextureCubeData > GetCachedTextureCubeData( const std::string& texCubeDataId );

        static std::shared_ptr< CTexture > GetCachedTexture( const std::string& texId );
        static std::shared_ptr< CTextureCube > GetCachedTextureCube( const std::string& texCubeId );

        static std::vector< std::shared_ptr< CTextureData > > GetAllCachedTexturesData();
        static std::vector< std::shared_ptr< CTexture > > GetAllCachedTextures();

    private :

        static CTextureManager* s_instance;

        CTextureManager();

        void _loadTextures();
        void _createBuiltInTextures();

        std::shared_ptr< CTextureData >     _loadTextureData( const std::string& filepath );
        std::shared_ptr< CTextureCubeData > _loadTextureCubeData( const std::array< std::string, 6 >& filepaths );

        std::shared_ptr< CTexture >     _loadTexture( const std::string& filepath, const CTextureOptions& texOptions );
        std::shared_ptr< CTextureCube > _loadTextureCube( const std::array< std::string, 6 >& filepaths );

        std::shared_ptr< CTextureData >     _getCachedTextureData( const std::string& texDataId );
        std::shared_ptr< CTextureCubeData > _getCachedTextureCubeData( const std::string& texCubeDataId );

        std::shared_ptr< CTexture >     _getCachedTexture( const std::string& texId );
        std::shared_ptr< CTextureCube > _getCachedTextureCube( const std::string& texCubeId );

        std::vector< std::shared_ptr< CTextureData > > const _getAllCachedTexturesData() { return m_texturesDataList; }
        std::vector< std::shared_ptr< CTextureCubeData > > const _getAllCachedTexturesCubeData() { return m_texturesCubeDataList; }

        std::vector< std::shared_ptr< CTexture > > const _getAllCachedTextures() { return m_texturesList; }
        std::vector< std::shared_ptr< CTextureCube > > const _getAllCachedTexturesCube() { return m_texturesCubeList; }

        std::vector< std::shared_ptr< CTextureData > >                  m_texturesDataList;
        std::vector< std::shared_ptr< CTextureCubeData > >              m_texturesCubeDataList;
        std::vector< std::shared_ptr< CTexture > >                      m_texturesList;
        std::vector< std::shared_ptr< CTextureCube > >                  m_texturesCubeList;

        std::map< std::string, std::shared_ptr< CTextureData > >        m_texturesData;
        std::map< std::string, std::shared_ptr< CTextureCubeData > >    m_texturesCubeData;
        std::map< std::string, std::shared_ptr< CTexture > >            m_textures;
        std::map< std::string, std::shared_ptr< CTextureCube > >        m_texturesCube;

    };

}