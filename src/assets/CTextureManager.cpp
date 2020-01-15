
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
        stbi_set_flip_vertically_on_load( true );

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

    CTextureData* CTextureManager::LoadTextureData( const std::string& filepath, bool flipVertically )
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_loadTextureData( filepath, flipVertically );
    }

    CTexture* CTextureManager::LoadTexture( const std::string& filepath,
                                            const CTextureOptions& texOptions,
                                            bool flipVertically )
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_loadTexture( filepath, texOptions, flipVertically );
    }

    CTexture* CTextureManager::LoadTexture( const std::string& filepath,
                                            const eTextureFilter& filterMin,
                                            const eTextureFilter& filterMag,
                                            const eTextureWrap& wrapU,
                                            const eTextureWrap& wrapV,
                                            const CVec4& borderColorU,
                                            const CVec4& borderColorV,
                                            const ePixelDataType& dtype,
                                            bool flipVertically )
    {
        return CTextureManager::LoadTexture( filepath, { filterMin, filterMag, wrapU, wrapV, borderColorU, borderColorV, dtype }, flipVertically );
    }

    CTextureData* CTextureManager::GetCachedTextureData( const std::string& texDataId )
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_getCachedTextureData( texDataId );
    }

    CTextureCubeData* CTextureManager::GetCachedTextureCubeData( const std::string& texCubeDataId )
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_getCachedTextureCubeData( texCubeDataId );
    }

    CTexture* CTextureManager::GetCachedTexture( const std::string& texId )
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_getCachedTexture( texId );
    }

    CTextureCube* CTextureManager::GetCachedTextureCube( const std::string& texCubeId )
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_getCachedTextureCube( texCubeId );
    }

    bool CTextureManager::HasCachedTextureData( const std::string& texDataId )
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_hasCachedTextureData( texDataId );
    }

    bool CTextureManager::HasCachedTextureCubeData( const std::string& texCubeDataId )
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_hasCachedTextureCubeData( texCubeDataId );
    }

    bool CTextureManager::HasCachedTexture( const std::string& texId )
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_hasCachedTexture( texId );
    }

    bool CTextureManager::HasCachedTextureCube( const std::string& texCubeId )
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_hasCachedTextureCube( texCubeId );
    }

    std::vector< CTextureData* > CTextureManager::GetAllCachedTexturesData()
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_getAllCachedTexturesData();
    }

    std::vector< CTexture* > CTextureManager::GetAllCachedTextures()
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_getAllCachedTextures();
    }

    std::vector< CTextureCubeData* > CTextureManager::GetAllCachedTexturesCubeData()
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_getAllCachedTexturesCubeData();
    }

    std::vector< CTextureCube* > CTextureManager::GetAllCachedTexturesCube()
    {
        ENGINE_CORE_ASSERT( CTextureManager::s_instance, "Must initialize texture manager before using it" );

        return CTextureManager::s_instance->_getAllCachedTexturesCube();
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
                    _texOptions.borderColorU    = CVec4( 0.0f, 0.0f, 0.0f, 1.0f );
                    _texOptions.borderColorV    = CVec4( 0.0f, 0.0f, 0.0f, 1.0f );
                    _texOptions.dtype           = ePixelDataType::UINT_8;

                    _loadTexture( _pathImagesFolder + _fname, _texOptions, true );
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
                _loadTextureCube( _filepaths, false );
            }
        }
    }

    CTextureData* CTextureManager::_loadTextureData( const std::string& filepath, bool flipVertically )
    {
        if ( flipVertically )
            stbi_set_flip_vertically_on_load( true );
        else
            stbi_set_flip_vertically_on_load( false );

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
            return m_texturesData[ _filenameNoExtension ].get();
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

        // keep ownership of the created texture data
        std::unique_ptr< CTextureData > _textureDataPtr( _textureData );
        m_texturesData[ _filenameNoExtension ] = std::move( _textureDataPtr );
        m_texturesDataList.push_back( _textureData );

        // default mode (vertically flipped)
        stbi_set_flip_vertically_on_load( true );

        return _textureData;
    }


    CTextureCubeData* CTextureManager::_loadTextureCubeData( const std::array< std::string, 6 >& filepaths, bool flipVertically )
    {
        if ( flipVertically )
            stbi_set_flip_vertically_on_load( true );
        else
            stbi_set_flip_vertically_on_load( false );

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
            return m_texturesCubeData[_name].get();
        }

        auto _textureCubeData = new CTextureCubeData();
        _textureCubeData->name              = _name;
        _textureCubeData->sidesData         = _sidesData;
        _textureCubeData->width             = _width;
        _textureCubeData->height            = _height;
        _textureCubeData->channels          = _channels;
        _textureCubeData->format            = ( _channels == 3 ) ? eTextureFormat::RGB : eTextureFormat::RGBA;

        // keep ownership of the created texture-cube data
        std::unique_ptr< CTextureCubeData > _textureCubeDataPtr( _textureCubeData );
        m_texturesCubeData[ _name ] = std::move( _textureCubeDataPtr );
        m_texturesCubeDataList.push_back( _textureCubeData );

        /* default mode (vertically flipped) */
        stbi_set_flip_vertically_on_load( true );

        return _textureCubeData;
    }

    CTexture* CTextureManager::_loadTexture( const std::string& filepath,
                                             const CTextureOptions& texOptions,
                                             bool flipVertically )
    {
        auto _textureData = _loadTextureData( filepath, flipVertically );
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
            return m_textures[ _filenameNoExtension ].get();
        }

        auto _texture = new CTexture( _textureData, texOptions );

        // keep ownership of the created texture
        std::unique_ptr< CTexture > _texturePtr( _texture );
        m_textures[ _filenameNoExtension ] = std::move( _texturePtr );
        m_texturesList.push_back( _texture );

        return _texture;
    }

    CTextureCube* CTextureManager::_loadTextureCube( const std::array< std::string, 6 >& filepaths,
                                                     bool flipVertically )
    {
        auto _textureCubeData = _loadTextureCubeData( filepaths, flipVertically );
        if ( !_textureCubeData )
        {
            ENGINE_CORE_ERROR( "Could not load requested texture-cube: {0}", filepaths.front() );
            return nullptr;
        }

        if ( m_textures.find( _textureCubeData->name ) != m_textures.end() )
        {
            ENGINE_CORE_WARN( "Tried loading texture-cube with name {0} twice. Returning cached one",
                              _textureCubeData->name );
            return m_texturesCube[ _textureCubeData->name ].get();
        }

        auto _textureCube = new CTextureCube( _textureCubeData );

        // keep ownership of the texture-cube
        std::unique_ptr< CTextureCube > _textureCubePtr( _textureCube );
        m_texturesCube[ _textureCubeData->name ] = std::move( _textureCubePtr );
        m_texturesCubeList.push_back( _textureCube );

        return _textureCube;
    }

    CTextureData* CTextureManager::_getCachedTextureData( const std::string& texDataId )
    {
        if ( m_texturesData.find( texDataId ) == m_texturesData.end() )
        {
            ENGINE_CORE_WARN( "Texture-data with id {0} hasn't been loaded yet.", texDataId );
            return nullptr;
        }

        return m_texturesData[ texDataId ].get();
    }

    CTextureCubeData* CTextureManager::_getCachedTextureCubeData( const std::string& texCubeDataId )
    {
        if ( m_texturesCubeData.find( texCubeDataId ) == m_texturesCubeData.end() )
        {
            ENGINE_CORE_WARN( "Texture-Cube-data with id {0} hasn't been loaded yet.", texCubeDataId );
            return nullptr;
        }

        return m_texturesCubeData[ texCubeDataId ].get();
    }

    CTexture* CTextureManager::_getCachedTexture( const std::string& texId )
    {
        if ( m_textures.find( texId ) == m_textures.end() )
        {
            ENGINE_CORE_WARN( "Texture with id {0} hasn't been loaded yet.", texId );
            return nullptr;
        }

        return m_textures[ texId ].get();
    }

    CTextureCube* CTextureManager::_getCachedTextureCube( const std::string& texCubeId )
    {
        if ( m_texturesCube.find( texCubeId ) == m_texturesCube.end() )
        {
            ENGINE_CORE_WARN( "Texture-Cube with id {0} hasn't been loaded yet", texCubeId );
            return nullptr;
        }

        return m_texturesCube[ texCubeId ].get();
    }

    bool CTextureManager::_hasCachedTextureData( const std::string& texDataId )
    {
        return m_texturesData.find( texDataId ) != m_texturesData.end();
    }

    bool CTextureManager::_hasCachedTextureCubeData( const std::string& texCubeDataId )
    {
        return m_texturesCubeData.find( texCubeDataId ) != m_texturesCubeData.end();
    }

    bool CTextureManager::_hasCachedTexture( const std::string& texId )
    {
        return m_textures.find( texId ) != m_textures.end();
    }

    bool CTextureManager::_hasCachedTextureCube( const std::string& texCubeId )
    {
        return m_texturesCube.find( texCubeId ) != m_texturesCube.end();
    }

    void CTextureManager::_createBuiltInTextures()
    {
        _createChessboardTexture();
    }

    void CTextureManager::_createChessboardTexture()
    {
        const size_t _width = 256;
        const size_t _height = 256;

        struct _CPixel
        {
            uint8 r; uint8 g; uint8 b;
        };

        auto _data = new _CPixel[ _width * _height ];

        for ( size_t i = 0; i < _height; i++ )
        {
            for ( size_t j = 0; j < _width; j++ )
            {
                if ( ( ( i & 0x40 ) == 0 ) ^ ( ( j & 0x40 ) == 0 ) )
                {
                    _data[ j + i * _width ].r = 255;
                    _data[ j + i * _width ].g = 255;
                    _data[ j + i * _width ].b = 255;
                }
                else
                {
                    _data[ j + i * _width ].r = 122;
                    _data[ j + i * _width ].g = 122;
                    _data[ j + i * _width ].b = 122;
                }
            }
        }

        auto _textureData = new CTextureData();
        _textureData->name              = "built_in_chessboard";
        _textureData->width             = _width;
        _textureData->height            = _height;
        _textureData->channels          = 3;
        _textureData->internalFormat    = eTextureFormat::RGB;
        _textureData->format            = eTextureFormat::RGB;
        _textureData->data              = (uint8*) _data;

        std::unique_ptr< CTextureData > _textureDataPtr( _textureData );
        m_texturesData[ "built_in_chessboard" ] = std::move( _textureDataPtr );
        m_texturesDataList.push_back( _textureData );

        auto _texture = new CTexture( _textureData,
                                      { eTextureFilter::NEAREST,
                                        eTextureFilter::NEAREST,
                                        eTextureWrap::REPEAT,
                                        eTextureWrap::REPEAT,
                                        CVec4( 0.0f, 0.0f, 0.0f, 1.0f ),
                                        CVec4( 0.0f, 0.0f, 0.0f, 1.0f ),
                                        ePixelDataType::UINT_8 } );

        std::unique_ptr< CTexture > _texturePtr( _texture );
        m_textures[ "built_in_chessboard" ] = std::move( _texturePtr );
        m_texturesList.push_back( _texture );

    }

}