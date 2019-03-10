
#include "LAssetsManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;

namespace engine
{

    LAssetsManager* LAssetsManager::_INSTANCE = NULL;
    string LAssetsManager::CUBE_VARIATIONS[] = { "front", "back", "top", "bottom", "right", "left" };

    LAssetsManager::LAssetsManager()
    {
        // Load some Cubemap textures
        m_cubeTextures["starfield"] = loadCubeTexture( "skyboxes/starfield" );
        // m_cubeTextures["clouds"]    = loadCubeTexture( "skyboxes/cloudtop" );

        _createBuiltInTextures();
    }

    void LAssetsManager::_createBuiltInTextures()
    {
        // Create chessboard pattern
        {
            int _w = 256;
            int _h = 256;

            struct LPixel
            {
                u8 r; u8 g; u8 b; u8 a;
            };

            auto _imgData = new LPixel[ _w * _h ];

            for ( int i = 0; i < _h; i++ )
            {
                for ( int j = 0; j < _w; j++ )
                {
                    if ( ( ( i & 0x20 ) == 0 ) ^ ( ( j & 0x20 ) == 0 ) )
                    {
                        _imgData[ j + i * _w ].r = 255;
                        _imgData[ j + i * _w ].g = 255;
                        _imgData[ j + i * _w ].b = 255;
                    }
                    else
                    {
                        _imgData[ j + i * _w ].r = 150;
                        _imgData[ j + i * _w ].g = 150;
                        _imgData[ j + i * _w ].b = 150;
                    }
                    _imgData[ j + i * _w ].a = 255;
                }
            }

            LTextureData _tdata;
            _tdata.data = ( u8* ) _imgData;
            _tdata.width = _w;
            _tdata.height = _h;
            _tdata.channels = 3;
            _tdata.ftype = GL_RGBA;

            m_builtInTextures["chessboard"] = new LTexture( _tdata );
        }
    }

    LAssetsManager::~LAssetsManager()
    {
        for ( auto _it = m_textures.begin(); 
                   _it != m_textures.end();
                   _it++ )
        {
            delete _it->second;
        }
        m_textures.clear();

        for ( auto _it = m_cubeTextures.begin();
                   _it != m_cubeTextures.end();
                   _it++ )
        {
            delete _it->second;
        }
        m_cubeTextures.clear();
    }

    void LAssetsManager::release()
    {
        if ( LAssetsManager::_INSTANCE != NULL )
        {
            delete LAssetsManager::_INSTANCE;
            LAssetsManager::_INSTANCE = NULL;
        }
    }

    void LAssetsManager::create()
    {
        if ( LAssetsManager::_INSTANCE != NULL )
        {
            delete LAssetsManager::_INSTANCE;
        }

        LAssetsManager::_INSTANCE = new LAssetsManager();
    }    

    LTextureData LAssetsManager::_loadTextureData( const string& filename, const string& ftype )
    {
        LTextureData _textureData;

        string _strFilename = string( ENGINE_RESOURCES_PATH ) + string( "imgs/" ) + filename + ftype;

        _textureData.data = stbi_load( _strFilename.c_str(), 
                                       &_textureData.width, 
                                       &_textureData.height,
                                       &_textureData.channels,
                                       0 );

        // cout << "loaded image: " << _strFilename << endl;
        // cout << "w: " << _textureData.width 
        //      << " - h: " << _textureData.height
        //      << " - c: " << _textureData.channels << endl;

        if ( ftype == ".jpg" || ftype == ".jpeg" )
        {
            // std::cout << "LOADED GL_RGB" << std::endl;
            _textureData.ftype = GL_RGB;
        }
        else if ( ftype == ".png" )
        {
            // std::cout << "LOADED GL_RGBA" << std::endl;
            _textureData.ftype = GL_RGBA;
        }
        else
        {
            cout << "WARNING> The image " << filename 
                 << " does not have a supported extension. " 
                 << "Using RGB by default." << endl;
            _textureData.ftype = GL_RGB;
        }

        return _textureData;
    }

    LCubeTextureData LAssetsManager::_loadCubeTextureData( const string& basename, const string& ftype )
    {
        LCubeTextureData _cubeTextureData;

        for ( int q = 0; q < 6; q++ )
        {
            string _resPath = string( basename ) + string( "_" ) + CUBE_VARIATIONS[q];
            auto _textureData = _loadTextureData( _resPath, ftype );

            _cubeTextureData.vpData[q] = _textureData.data;
            _cubeTextureData.width = _textureData.width;
            _cubeTextureData.height = _textureData.height;
            _cubeTextureData.channels = _textureData.channels;
        }

        if ( ftype == ".jpg" || ftype == ".jpeg" )
        {
            // std::cout << "LOADED GL_RGB" << std::endl;
            _cubeTextureData.ftype = GL_RGB;
        }
        else if ( ftype == ".png" )
        {
            // std::cout << "LOADED GL_RGBA" << std::endl;
            _cubeTextureData.ftype = GL_RGBA;
        }
        else
        {
            cout << "WARNING> The cube-image " << basename 
                 << " does not have a supported extension. " 
                 << "Using RGB by default." << endl;
            _cubeTextureData.ftype = GL_RGB;
        }

        return _cubeTextureData;
    }

    LTexture* LAssetsManager::loadTexture( const string& filename, const string& ftype )
    {
        return new LTexture( _loadTextureData( filename, ftype ) );
    }

    LCubeTexture* LAssetsManager::loadCubeTexture( const string& basename, const string& ftype )
    {
        return new LCubeTexture( _loadCubeTextureData( basename, ftype ) );
    }

    LTexture* LAssetsManager::getTexture( const string& textureId )
    {
        if ( _INSTANCE->m_textures.find( textureId ) == _INSTANCE->m_textures.end() )
        {
            cout << "WARNING> Texture with id: " << textureId << " hasn't been loaded" << endl;
            return NULL;
        }

        return _INSTANCE->m_textures[textureId];
    }

    LCubeTexture* LAssetsManager::getCubeTexture( const string& cubeTextureId )
    {
        if ( _INSTANCE->m_cubeTextures.find( cubeTextureId ) == _INSTANCE->m_cubeTextures.end() )
        {
            cout << "WARNING> CubeTexture with id: " << cubeTextureId << " hasn't been loaded" << endl;
            return NULL;
        }

        return _INSTANCE->m_cubeTextures[cubeTextureId];
    }

    LTexture* LAssetsManager::getBuiltInTexture( const string& textureId )
    {
         if ( _INSTANCE->m_builtInTextures.find( textureId ) == _INSTANCE->m_builtInTextures.end() )
        {
            cout << "WARNING> Texture with id: " << textureId << " is not a built-in texture" << endl;
            return NULL;
        }

        return _INSTANCE->m_builtInTextures[textureId];
    }

}