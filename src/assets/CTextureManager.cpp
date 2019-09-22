
#include <assets/CTextureManager.h>

#include <stb_image.h>

namespace engine
{

    CTextureManager* CTextureManager::s_instance = nullptr;

    void CTextureManager::Init()
    {
        if ( CTextureManager::s_instance )
            return;

        CTextureManager::s_instance = new CTextureManager();

        ENGINE_CORE_INFO( "Initialized texture-manager" );
    }

    void CTextureManager::Release()
    {
        if ( !CTextureManager::s_instance )
            ENGINE_CORE_ASSERT( false, "Tried to release tex-manager more than once" );

        delete CTextureManager::s_instance;
        s_instance = nullptr;
    }

    CTextureManager::CTextureManager()
    {
        _loadTextures();
        _createBuiltInTextures();
    }

    CTextureManager::~CTextureManager()
    {
        m_textures.clear();
        m_texturesData.clear();
    }

    std::shared_ptr< CTextureData > CTextureManager::LoadTextureData( const std::string& filepath )
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_loadTextureData( filepath );
    }

    std::shared_ptr< CTexture > CTextureManager::LoadTexture( const std::string& filepath,
                                                              const CTextureOptions& texOptions )
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_loadTexture( filepath, texOptions );
    }

    std::shared_ptr< CTexture > CTextureManager::LoadTexture( const std::string& filepath,
                                                              const eTextureFilter& filterMin,
                                                              const eTextureFilter& filterMag,
                                                              const eTextureWrap& wrapU,
                                                              const eTextureWrap& wrapV,
                                                              const CVec4& borderColorU,
                                                              const CVec4& borderColorV,
                                                              uint32 textureUnit )
    {
        return CTextureManager::LoadTexture( filepath, { filterMin, filterMag, wrapU, wrapV, borderColorU, borderColorV, textureUnit } );
    }

    std::shared_ptr< CTextureData > CTextureManager::GetCachedTextureData( const std::string& texdataId )
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_getCachedTextureData( texdataId );
    }

    std::shared_ptr< CTexture > CTextureManager::GetCachedTexture( const std::string& texId )
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_getCachedTexture( texId );
    }

    void CTextureManager::_loadTextures()
    {
        std::string _pathImagesFolder = std::string( ENGINE_RESOURCES_PATH ) + "imgs/";
        std::string _pathSkyboxesFolder = std::string( ENGINE_RESOURCES_PATH ) + "imgs/skyboxes/";

        DIR* _directoryPtr;
        struct dirent* _direntPtr;

        _directoryPtr = opendir( _pathImagesFolder.c_str() );
        if ( _directoryPtr )
        {
            while( _direntPtr = readdir( _directoryPtr ) )
            {
                std::string _fname = _direntPtr->d_name;
                if ( _fname.find( ".jpg" ) != std::string::npos ||
                     _fname.find( ".jpeg" ) != std::string::npos ||
                     _fname.find( ".png" ) != std::string::npos )
                {
                    CTextureOptions _texOptions;
                    _texOptions.filterMin       = eTextureFilter::NEAREST;
                    _texOptions.filterMag       = eTextureFilter::NEAREST;
                    _texOptions.wrapU           = eTextureWrap::REPEAT;
                    _texOptions.wrapV           = eTextureWrap::REPEAT;
                    _texOptions.borderColorU    = { 0.0f, 0.0f, 0.0f, 1.0f };
                    _texOptions.borderColorV    = { 0.0f, 0.0f, 0.0f, 1.0f };
                    _texOptions.textureUnit     = 0;

                    _loadTexture( _pathImagesFolder + _fname, _texOptions );
                }
            }
            closedir( _directoryPtr );
        }

        // @TODO: Add functionality for cube-textures
    }

    std::shared_ptr< CTextureData > CTextureManager::_loadTextureData( const std::string& filepath )
    {
        ePixelFormat _format = ePixelFormat::NONE;
        if ( filepath.find( ".jpg" ) != std::string::npos ) { _format = ePixelFormat::RGB; }
        if ( filepath.find( ".jpeg" ) != std::string::npos ) { _format = ePixelFormat::RGB; }
        if ( filepath.find( ".png" ) != std::string::npos ) { _format = ePixelFormat::RGBA; }

        if ( _format == ePixelFormat::NONE )
        {
            ENGINE_CORE_WARN( "Tried loading image %s that hasn't a supported extension", filepath );
            return nullptr;
        }

        std::string _filename = engine::split( filepath, '/' ).back();
        std::string _filenameNoExtension = engine::split( _filename, '.' ).front();

        auto _textureData = new CTextureData();

        int _width, _height, _channels;
        _textureData->data = stbi_load( filepath.c_str(), &_width, &_height, &_channels, 0 );
        if ( !_textureData->data )
        {
            ENGINE_CORE_ERROR( "Could not load image %s", filepath );
            return nullptr;
        }

        _textureData->format    = _format;
        _textureData->width     = _width;
        _textureData->height    = _height;
        _textureData->channels  = _channels;

        std::shared_ptr< CTextureData > _textureDataPtr;
        _textureDataPtr.reset( _textureData );

        m_texturesData[ _filenameNoExtension ] = _textureDataPtr;

        return _textureDataPtr;
    }

    std::shared_ptr< CTexture > CTextureManager::_loadTexture( const std::string& filepath,
                                                               const CTextureOptions& texOptions )
    {
        auto _textureData = _loadTextureData( filepath );
        if ( !_textureData )
        {
            ENGINE_CORE_WARN( "Could not load texture %s with not-supported file extension", filepath );
            return nullptr;
        }

        std::string _filename = engine::split( filepath, '/' ).back();
        std::string _filenameNoExtension = engine::split( _filename, '.' ).front();

        auto _texture = new CTexture( _textureData, texOptions );

        std::shared_ptr< CTexture > _texturePtr;
        _texturePtr.reset( _texture );

        m_textures[ _filenameNoExtension ] = _texturePtr;

        return _texturePtr;
    }

    std::shared_ptr< CTextureData > CTextureManager::_getCachedTextureData( const std::string& texdataId )
    {
        if ( m_texturesData.find( texdataId ) == m_texturesData.end() )
        {
            ENGINE_CORE_WARN( "Texture-data with id %s hasn't been loaded yet.", texdataId );
            return nullptr;
        }

        return m_texturesData[ texdataId ];
    }

    std::shared_ptr< CTexture > CTextureManager::_getCachedTexture( const std::string& texId )
    {
        if ( m_textures.find( texId ) == m_textures.end() )
        {
            ENGINE_CORE_WARN( "Texture with id %s hasn't been loaded yet.", texId );
            return nullptr;
        }

        return m_textures[ texId ];
    }


    void CTextureManager::_createBuiltInTextures()
    {
        // @TODO: Add functionality for built-in textures
    }

}