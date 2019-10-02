
#include <assets/CTextureManager.h>

#define STB_IMAGE_IMPLEMENTATION
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
        // make sure textures are loaded in the standard orientation
        // stbi_set_flip_vertically_on_load( true );

        _loadTextures();
        _createBuiltInTextures();
    }

    CTextureManager::~CTextureManager()
    {
        m_textures.clear();
        m_texturesList.clear();

        m_texturesData.clear();
        m_texturesDataList.clear();
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
                                                              const ePixelDataType& dtype,
                                                              uint32 textureUnit )
    {
        return CTextureManager::LoadTexture( filepath, { filterMin, filterMag, wrapU, wrapV, borderColorU, borderColorV, dtype, textureUnit } );
    }

    std::shared_ptr< CTextureData > CTextureManager::GetCachedTextureData( const std::string& texdataId )
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_getCachedTextureData( texdataId );
    }

    std::shared_ptr< CTextureCubeData > CTextureManager::GetCachedTextureCubeData( const std::string& texCubeDataId )
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_getCachedTextureCubeData( texCubeDataId );
    }

    std::shared_ptr< CTexture > CTextureManager::GetCachedTexture( const std::string& texId )
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_getCachedTexture( texId );
    }

    std::shared_ptr< CTextureCube > CTextureManager::GetCachedTextureCube( const std::string& texCubeId )
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_getCachedTextureCube( texCubeId );
    }

    std::vector< std::shared_ptr< CTextureData > > CTextureManager::GetAllCachedTexturesData()
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_getAllCachedTexturesData();
    }

    std::vector< std::shared_ptr< CTexture > > CTextureManager::GetAllCachedTextures()
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_getAllCachedTextures();
    }


    void CTextureManager::_loadTextures()
    {
        /* Load 2D-textures from disk (resources directory of the engine)*/
        std::string _pathImagesFolder = std::string( ENGINE_RESOURCES_PATH ) + "imgs/";

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
                     _fname.find( ".png" ) != std::string::npos ||
                     _fname.find( ".tga" ) != std::string::npos )
                {
                    CTextureOptions _texOptions;
                    _texOptions.filterMin       = eTextureFilter::LINEAR;
                    _texOptions.filterMag       = eTextureFilter::LINEAR;
                    _texOptions.wrapU           = eTextureWrap::REPEAT;
                    _texOptions.wrapV           = eTextureWrap::REPEAT;
                    _texOptions.borderColorU    = { 0.0f, 0.0f, 0.0f, 1.0f };
                    _texOptions.borderColorV    = { 0.0f, 0.0f, 0.0f, 1.0f };
                    _texOptions.dtype           = ePixelDataType::UINT_8;
                    _texOptions.textureUnit     = 0;

                    _loadTexture( _pathImagesFolder + _fname, _texOptions );
                }
            }
            closedir( _directoryPtr );
        }

        /* Load 3D-textures from disk (resources directory of the engine)*/
        std::string _pathSkyboxesFolder = std::string( ENGINE_RESOURCES_PATH ) + "imgs/skyboxes/";

        _directoryPtr = opendir( _pathSkyboxesFolder.c_str() );
        if ( _directoryPtr )
        {
            // resources to be loaded by the engine for 3D-Textures must have the following
            // name standard: 
            //      (BASE-NAME)_(back|front|bottom|top|left|right).(jpg|jpeg|png)
            // the general case is
            //      (BASE-NAME)_(SIDE-VARIATION).(jpg|jpeg|png)
            // so the user might define its own variation apart from the ones we use. Still,
            // for the default textures used by the engine, we are using the first format

            std::map< std::string, std::string > _mapBasenameExtension;

            while( _direntPtr = readdir( _directoryPtr ) )
            {
                std::string _fname = _direntPtr->d_name;
                if ( _fname.find( ".jpg" ) != std::string::npos ||
                     _fname.find( ".jpeg" ) != std::string::npos ||
                     _fname.find( ".png" ) != std::string::npos ||
                     _fname.find( ".tga" ) != std::string::npos )
                {
                    std::string _filename = engine::split( _fname, '/' ).back();
                    std::string _filenameNoExtension = engine::split( _filename, '.' ).front();
                    std::string _fileExtension = engine::split( _filename, '.' ).back();
                    std::string _filenameBasename = engine::split( _filenameNoExtension, '_' ).front();

                    _mapBasenameExtension[ _filenameBasename ] = _fileExtension;
                }
            }

            closedir( _directoryPtr );

            for ( auto it : _mapBasenameExtension )
            {
                std::array< std::string, 6 > _filepaths = { _pathSkyboxesFolder + it.first + "_front." + it.second, 
                                                            _pathSkyboxesFolder + it.first + "_back." + it.second,
                                                            _pathSkyboxesFolder + it.first + "_top." + it.second, 
                                                            _pathSkyboxesFolder + it.first + "_bottom." + it.second,
                                                            _pathSkyboxesFolder + it.first + "_right." + it.second, 
                                                            _pathSkyboxesFolder + it.first + "_left." + it.second };
                _loadTextureCube( _filepaths );
            }
        }
    }

    std::shared_ptr< CTextureData > CTextureManager::_loadTextureData( const std::string& filepath )
    {
        eTextureFormat _format = eTextureFormat::NONE;
        if ( filepath.find( ".jpg" ) != std::string::npos ) { _format = eTextureFormat::RGB; }
        if ( filepath.find( ".jpeg" ) != std::string::npos ) { _format = eTextureFormat::RGB; }
        if ( filepath.find( ".png" ) != std::string::npos ) { _format = eTextureFormat::RGBA; }

        if ( _format == eTextureFormat::NONE )
        {
            ENGINE_CORE_WARN( "Tried loading image {0} that hasn't a supported extension", filepath );
            return nullptr;
        }

        std::string _filename = engine::split( filepath, '/' ).back();
        std::string _filenameNoExtension = engine::split( _filename, '.' ).front();

        if ( m_texturesData.find( _filenameNoExtension ) != m_texturesData.end() )
        {
            ENGINE_CORE_WARN( "Tried loading texture-data with name {0} twice. Returning cached one", 
                              _filenameNoExtension );
            return m_texturesData[ _filenameNoExtension ];
        }

        auto _textureData = new CTextureData();

        int _width, _height, _channels;
        _textureData->data = stbi_load( filepath.c_str(), &_width, &_height, &_channels, 0 );
        if ( !_textureData->data )
        {
            ENGINE_CORE_ERROR( "Could not load image {0}", filepath );
            return nullptr;
        }

        _textureData->name              = _filenameNoExtension;
        _textureData->width             = _width;
        _textureData->height            = _height;
        _textureData->channels          = _channels;
        _textureData->internalFormat    = ( _channels == 3 ) ? eTextureFormat::RGB : eTextureFormat::RGBA;
        _textureData->format            = ( _channels == 3 ) ? eTextureFormat::RGB : eTextureFormat::RGBA;

        std::shared_ptr< CTextureData > _textureDataPtr;
        _textureDataPtr.reset( _textureData );

        m_texturesData[ _filenameNoExtension ] = _textureDataPtr;
        m_texturesDataList.push_back( _textureDataPtr );

        return _textureDataPtr;
    }


    std::shared_ptr< CTextureCubeData > CTextureManager::_loadTextureCubeData( const std::array< std::string, 6 >& filepaths )
    {

        bool _errorFormat           = false;
        bool _errorData             = false;
        bool _errorFileStandard     = false;
        bool _errorFormatMismatch   = false;
        bool _warnAlreadyCached     = false;
        bool _warnSizeMismatch      = false;
        bool _warnBaseNameMismatch  = false;
        std::string _issueMessage   = "";

        std::string _name                   = "undefined";
        std::array< uint8*, 6 > _sidesData  = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
        int _width                          = 0;
        int _height                         = 0;
        int _channels                       = 0;
        eTextureFormat _format                = eTextureFormat::NONE;

        for ( size_t i = 0; i < 6; i++ )
        {
            eTextureFormat _sideFormat = eTextureFormat::NONE;
            if ( filepaths[i].find( ".jpg" ) != std::string::npos ) { _sideFormat = eTextureFormat::RGB; }
            if ( filepaths[i].find( ".jpeg" ) != std::string::npos ) { _sideFormat = eTextureFormat::RGB; }
            if ( filepaths[i].find( ".png" ) != std::string::npos ) { _sideFormat = eTextureFormat::RGBA; }
            if ( filepaths[i].find( ".tga" ) != std::string::npos ) { _sideFormat = eTextureFormat::RGBA; }

            if ( _sideFormat == eTextureFormat::NONE )
            {
                _errorFormat = true;
                _issueMessage = std::string( "Tried loading texture-cube data with " ) +
                                std::string( "unsupported format extension from file: " ) + filepaths[i];
                break;
            }

            if ( ( i > 0 ) && ( _format != _sideFormat ) ) 
            {
                _errorFormatMismatch = true;
                _issueMessage = std::string( "Format of side file: " ) + filepaths[i] + std::string( " " ) +
                                std::string( "does not the format of the other files." );
                break;
            }

            _format = _sideFormat;

            auto _filename = engine::split( filepaths[i], '/' ).back();
            auto _filenameNoExtension = engine::split( _filename, '.' ).front();
            auto _filenameNoExtensionSplits = engine::split( _filenameNoExtension, '_' );
            auto _filenameBasename = _filenameNoExtensionSplits.front();

            if ( _filenameNoExtensionSplits.size() != 2 )
            {
                _errorFileStandard = true;
                _issueMessage = std::string( "Non-standard name given while loading texture-cube data: " ) +
                                filepaths[i] + std::string( ". It should be of standard: (BASE)_(VARIATION).(EXTENSION) " ) +
                                std::string( "with no extra '_' characters." );
                break;
            }

            if ( ( i > 0 ) && ( _name != _filenameBasename ) )
            {
                _warnBaseNameMismatch = true;
                _issueMessage = std::string( "Base name of file: " ) + filepaths[i] + std::string( " does not match the other sides' basename" );
            }

            _name = _filenameBasename;

            if ( m_texturesCubeData.find( _filenameBasename ) != m_texturesCubeData.end() )
            {
                _warnAlreadyCached = true;
                _issueMessage = std::string( "It seems that there's already a cached cube-texture " ) +
                                std::string( "data struct for:" ) + _filenameBasename + std::string( ". " ) +
                                std::string( "Returning cached one " );
                break;
            }

            int _sideWidth; int _sideHeight; int _sideChannels;

            uint8* _sideData = stbi_load( filepaths[i].c_str(), &_sideWidth, &_sideHeight, &_sideChannels, 0 );

            if ( !_sideData )
            {
                _errorData = true;
                _issueMessage = std::string( "Couldn't load data from file: " ) + filepaths[i];
                break;;
            }

            if ( ( i > 0 ) && ( _sideWidth != _width || _sideHeight != _height || _sideChannels != _channels ) )
            {
                _warnSizeMismatch = true;
                _issueMessage = std::string( "File: " ) + filepaths[i] + std::string( " seems to have different " ) +
                                std::string( "size compared to the other sides of the cube-texture data being loaded." );
            }

            // book-keeping for later creation of texture-cube data
            _width          = _sideWidth;
            _height         = _sideHeight;
            _channels       = _sideChannels;
            _sidesData[i]   = _sideData;
        }

        // clean after ourselves if something went wrong when loading the 6 tex-sides
        if ( _errorData || _errorFormat || _errorFileStandard || _warnAlreadyCached )
            for ( size_t i = 0; i < 6; i++ )
                if ( _sidesData[i] )
                    delete _sidesData[i];

        if ( _errorData || _errorFormat || _errorFileStandard || _errorFormatMismatch )
        {
            ENGINE_CORE_ERROR( _issueMessage );
            return nullptr;
        }

        if ( _warnSizeMismatch || _warnBaseNameMismatch  )
            ENGINE_CORE_WARN( _issueMessage );

        if ( _warnAlreadyCached )
        {
            ENGINE_CORE_WARN( _issueMessage );
            return m_texturesCubeData[ _name ];
        }

        auto _textureCubeData = new CTextureCubeData();
        _textureCubeData->name              = _name;
        _textureCubeData->sidesData         = _sidesData;
        _textureCubeData->width             = _width;
        _textureCubeData->height            = _height;
        _textureCubeData->channels          = _channels;
        _textureCubeData->format            = ( _channels == 3 ) ? eTextureFormat::RGB : eTextureFormat::RGBA;

        std::shared_ptr< CTextureCubeData > _textureCubeDataPtr;
        _textureCubeDataPtr.reset( _textureCubeData );

        m_texturesCubeData[ _name ] = _textureCubeDataPtr;
        m_texturesCubeDataList.push_back( _textureCubeDataPtr );

        return _textureCubeDataPtr;
    }

    std::shared_ptr< CTexture > CTextureManager::_loadTexture( const std::string& filepath,
                                                               const CTextureOptions& texOptions )
    {
        auto _textureData = _loadTextureData( filepath );
        if ( !_textureData )
        {
            ENGINE_CORE_ERROR( "Could not load requested texture: {0}", filepath );
            return nullptr;
        }

        std::string _filename = engine::split( filepath, '/' ).back();
        std::string _filenameNoExtension = engine::split( _filename, '.' ).front();

        if ( m_textures.find( _filenameNoExtension ) != m_textures.end() )
        {
            ENGINE_CORE_WARN( "Tried loading texture with name {0} twice. Returning cached one", 
                              _filenameNoExtension );
            return m_textures[ _filenameNoExtension ];
        }

        auto _texture = new CTexture( _textureData, texOptions );

        std::shared_ptr< CTexture > _texturePtr;
        _texturePtr.reset( _texture );

        m_textures[ _filenameNoExtension ] = _texturePtr;
        m_texturesList.push_back( _texturePtr );

        return _texturePtr;
    }

    std::shared_ptr< CTextureCube > CTextureManager::_loadTextureCube( const std::array< std::string, 6 >& filepaths )
    {
        auto _textureCubeData = _loadTextureCubeData( filepaths );
        if ( !_textureCubeData )
        {
            ENGINE_CORE_ERROR( "Could not load requested texture-cube: {0}", filepaths.front() );
            return nullptr;
        }

        if ( m_textures.find( _textureCubeData->name ) != m_textures.end() )
        {
            ENGINE_CORE_WARN( "Tried loading texture-cube with name {0} twice. Returning cached one",
                              _textureCubeData->name );
            return m_texturesCube[ _textureCubeData->name ];
        }

        auto _textureCube = new CTextureCube( _textureCubeData );

        std::shared_ptr< CTextureCube > _textureCubePtr;
        _textureCubePtr.reset( _textureCube );

        m_texturesCube[ _textureCubeData->name ] = _textureCubePtr;
        m_texturesCubeList.push_back( _textureCubePtr );

        return _textureCubePtr;
    }

    std::shared_ptr< CTextureData > CTextureManager::_getCachedTextureData( const std::string& texdataId )
    {
        if ( m_texturesData.find( texdataId ) == m_texturesData.end() )
        {
            ENGINE_CORE_WARN( "Texture-data with id {0} hasn't been loaded yet.", texdataId );
            return nullptr;
        }

        return m_texturesData[ texdataId ];
    }

    std::shared_ptr< CTextureCubeData > CTextureManager::_getCachedTextureCubeData( const std::string& texCubeDataId )
    {
        if ( m_texturesCubeData.find( texCubeDataId ) == m_texturesCubeData.end() )
        {
            ENGINE_CORE_WARN( "Texture-Cube-data with id {0} hasn't been loaded yet.", texCubeDataId );
            return nullptr;
        }

        return m_texturesCubeData[ texCubeDataId ];
    }

    std::shared_ptr< CTexture > CTextureManager::_getCachedTexture( const std::string& texId )
    {
        if ( m_textures.find( texId ) == m_textures.end() )
        {
            ENGINE_CORE_WARN( "Texture with id {0} hasn't been loaded yet.", texId );
            return nullptr;
        }

        return m_textures[ texId ];
    }

    std::shared_ptr< CTextureCube > CTextureManager::_getCachedTextureCube( const std::string& texCubeId )
    {
        if ( m_texturesCube.find( texCubeId ) == m_texturesCube.end() )
        {
            ENGINE_CORE_WARN( "Texture-Cube with id {0} hasn't been loaded yet", texCubeId );
            return nullptr;
        }

        return m_texturesCube[ texCubeId ];
    }

    void CTextureManager::_createBuiltInTextures()
    {
        // @TODO: Add functionality for built-in textures
    }

}