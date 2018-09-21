#pragma once

#include <LCommon.h>
#include "../Config.h"

#include <LTexture.h>
#include <LCubeTexture.h>

using namespace std;

namespace engine
{

    class LAssetsManager
    {
        private :

        LAssetsManager();

        static string CUBE_VARIATIONS[6];
        
        map< string, LTexture* > m_textures;
        map< string, LCubeTexture* > m_cubeTextures;
        map< string, LTexture* > m_builtInTextures;

        static LAssetsManager* _INSTANCE;

        void _createBuiltInTextures();

        public :

        static void create();
        static void release();

        ~LAssetsManager();

        static LTextureData _loadTextureData( const string& filename,
                                              const string& ftype = ".jpg" );
        static LCubeTextureData _loadCubeTextureData( const string& basename,
                                                  const string& ftype = ".jpg" );

        static LTexture* loadTexture( const string& filename, 
                                      const string& ftype = ".jpg" );
        static LCubeTexture* loadCubeTexture( const string& basename, 
                                              const string& ftype = ".jpg" );
        static LTexture* getTexture( const string& textureId );
        static LCubeTexture* getCubeTexture( const string& cubeTextureId );

        static LTexture* getBuiltInTexture( const string& textureId );
    };



}