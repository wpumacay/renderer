
#include "LAssetsManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;

namespace engine
{

    LAssetsManager* LAssetsManager::INSTANCE = NULL;
    string LAssetsManager::s_cubeVariations[] = { "right", "left", "top", "bottom", "back", "front" };

    LAssetsManager::LAssetsManager()
    {
        //stbi_set_flip_vertically_on_load( true );

        // Load some 2d texture assets

        texturesData["brickwall"]    = loadTexture( "img_brickwall" );
        texturesData["floor"]    = loadTexture( "img_brickwall_low" );
        
        texturesData["grass"]        = loadTexture( "grass_low" );
        texturesData["grass_sandy"]  = loadTexture( "grass_sandy_low" );
        texturesData["ground_stony"] = loadTexture( "ground_stony_low" );
        texturesData["rocks_1"]      = loadTexture( "rocks_1_low" );
        texturesData["rocks_2"]      = loadTexture( "rocks_2_low" );
        texturesData["snow"]         = loadTexture( "snow_low" );
        texturesData["water"]        = loadTexture( "water_low" );


        // Load some Cubemaps textures

        LCubeTextureData* _cubeTextureData;

        _cubeTextureData = loadCubeTexture( "cloudtop" );

        cubeTexturesData["space"] = _cubeTextureData;
    }

    LAssetsManager::~LAssetsManager()
    {
        release();
    }

    void LAssetsManager::release()
    {
        for ( auto _it = texturesData.begin(); 
                   _it != texturesData.end();
                   _it++ )
        {
            delete _it->second;
        }
    }

    void LAssetsManager::create()
    {
        if ( LAssetsManager::INSTANCE != NULL )
        {
            delete LAssetsManager::INSTANCE;
        }

        LAssetsManager::INSTANCE = new LAssetsManager();
    }    

    LTextureData* LAssetsManager::loadTexture( const char* filename, const char* ftype )
    {
        LTextureData* _textureData = new LTextureData();

        string _strFilename = string( ENGINE_RESOURCES_PATH ) + string( "imgs/" ) + string( filename ) + ftype;

        _textureData->data = stbi_load( _strFilename.c_str(), 
                                    &_textureData->width, 
                                    &_textureData->height,
                                    &_textureData->channels,
                                    0 );

        cout << "loaded image: " << _strFilename << endl;
        cout << "w: " << _textureData->width 
             << " - h: " << _textureData->height
             << " - c: " << _textureData->channels << endl;

        return _textureData;
    }

    LCubeTextureData* LAssetsManager::loadCubeTexture( const char* basename, const char* ftype )
    {
        LCubeTextureData* _cubeTextureData = new LCubeTextureData();

        for ( int q = 0; q < 6; q++ )
        {
            string _resPath = string( basename ) + string( "_" ) + s_cubeVariations[q];
            auto _textureData = loadTexture( _resPath.c_str(), ftype );

            _cubeTextureData->vpData[q] = _textureData->data;
            _cubeTextureData->width = _textureData->width;
            _cubeTextureData->height = _textureData->height;
            _cubeTextureData->channels = _textureData->channels;
        }


        return _cubeTextureData;
    }

}