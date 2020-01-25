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

        static CTexture* LoadTexture( const std::string& filepath,
                                      const eTextureFilter& filterMin = eTextureFilter::NEAREST,
                                      const eTextureFilter& filterMag = eTextureFilter::NEAREST,
                                      const eTextureWrap& wrapU = eTextureWrap::REPEAT,
                                      const eTextureWrap& wrapV = eTextureWrap::REPEAT,
                                      const CVec4& borderColorU = CVec4( 0.0f, 0.0f, 0.0f, 1.0f ),
                                      const CVec4& borderColorV = CVec4( 0.0f, 0.0f, 0.0f, 1.0f ),
                                      const ePixelDataType& dtype = ePixelDataType::UINT_8,
                                      bool flipVertically = true );

        static CTexture* LoadTexture( const std::string& filepath,
                                      const CTextureOptions& texOptions,
                                      bool flipVertically = true );

        static CTextureCube* LoadTextureCube( const std::array< std::string, 6 >& filepaths, bool flipVertically = false );

        static CTextureData* GetCachedTextureData( const std::string& texDataId );
        static CTextureCubeData* GetCachedTextureCubeData( const std::string& texCubeDataId );

        static CTexture* GetCachedTexture( const std::string& texId );
        static CTextureCube* GetCachedTextureCube( const std::string& texCubeId );

        static bool HasCachedTextureData( const std::string& texDataId );
        static bool HasCachedTextureCubeData( const std::string& texCubeDataId );

        static bool HasCachedTexture( const std::string& texId );
        static bool HasCachedTextureCube( const std::string& texCubeId );

        static std::vector< CTextureData* > GetAllCachedTexturesData();
        static std::vector< CTexture* > GetAllCachedTextures();

        static std::vector< CTextureCubeData* > GetAllCachedTexturesCubeData();
        static std::vector< CTextureCube* > GetAllCachedTexturesCube();

    private :

        static CTextureManager* s_instance;

        CTextureManager();

        void _loadTextures();
        void _createBuiltInTextures();
        void _createChessboardTexture();

        std::unique_ptr<CTextureData> _loadTextureData( const std::string& filepath, bool flipVertically );
        std::unique_ptr<CTextureCubeData> _loadTextureCubeData( const std::array< std::string, 6 >& filepaths, bool flipVertically );

        CTexture* _loadTexture( const std::string& filepath, const CTextureOptions& texOptions, bool flipVertically );
        CTextureCube* _loadTextureCube( const std::array< std::string, 6 >& filepaths, bool flipVertically );

        CTextureData* _getCachedTextureData( const std::string& texDataId );
        CTextureCubeData* _getCachedTextureCubeData( const std::string& texCubeDataId );

        CTexture* _getCachedTexture( const std::string& texId );
        CTextureCube* _getCachedTextureCube( const std::string& texCubeId );

        bool _hasCachedTextureData( const std::string& texDataId );
        bool _hasCachedTextureCubeData( const std::string& texCubeDataId );

        bool _hasCachedTexture( const std::string& texId );
        bool _hasCachedTextureCube( const std::string& texCubeId );

        std::vector< CTextureData* > const _getAllCachedTexturesData() { return m_texturesDataListRefs; }
        std::vector< CTextureCubeData* > const _getAllCachedTexturesCubeData() { return m_texturesCubeDataListRefs; }

        std::vector< CTexture* > const _getAllCachedTextures() { return m_texturesListRefs; }
        std::vector< CTextureCube* > const _getAllCachedTexturesCube() { return m_texturesCubeListRefs; }

        std::vector< CTextureData* >     m_texturesDataListRefs;
        std::vector< CTextureCubeData* > m_texturesCubeDataListRefs;
        std::vector< CTexture* >         m_texturesListRefs;
        std::vector< CTextureCube* >     m_texturesCubeListRefs;

        std::map< std::string, CTextureData* >                      m_texturesDataMapRefs;
        std::map< std::string, CTextureCubeData* >                  m_texturesCubeDataMapRefs;
        std::map< std::string, std::unique_ptr< CTexture > >        m_texturesMap;
        std::map< std::string, std::unique_ptr< CTextureCube > >    m_texturesCubeMap;

    };

}