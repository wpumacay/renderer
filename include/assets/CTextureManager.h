#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <core/CTexture.h>

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
                                                        uint32 textureUnit = 0 );
        static std::shared_ptr< CTexture > LoadTexture( const std::string& filepath,
                                                        const CTextureOptions& texOptions );

        static std::shared_ptr< CTextureData > GetCachedTextureData( const std::string& texdataId );
        static std::shared_ptr< CTexture > GetCachedTexture( const std::string& texId );

    private :

        CTextureManager();

        void _loadTextures();
        void _createBuiltInTextures();

        std::shared_ptr< CTextureData > _loadTextureData( const std::string& filepath );
        std::shared_ptr< CTexture > _loadTexture( const std::string& filepath,
                                                  const CTextureOptions& texOptions );

        std::shared_ptr< CTextureData > _getCachedTextureData( const std::string& texdataId );
        std::shared_ptr< CTexture > _getCachedTexture( const std::string& texId );

        static CTextureManager* s_instance;

        std::map< std::string, std::shared_ptr< CTextureData > >    m_texturesData;
        std::map< std::string, std::shared_ptr< CTexture > >        m_textures;

    };

}