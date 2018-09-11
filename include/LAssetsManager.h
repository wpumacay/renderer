#pragma once

#include "LCommon.h"
#include "../Config.h"

#include <string>
#include <map>

using namespace std;

namespace engine
{

    struct LAssetsManager
    {

        private :

        LAssetsManager();

        static string s_cubeVariations[6];

        public :

        map< string, LTextureData* > texturesData;
        map< string, LCubeTextureData* > cubeTexturesData;

        static LAssetsManager* INSTANCE;

        static void create();
        ~LAssetsManager();
        void release();

        LTextureData* loadTexture( const char* filename, const char* ftype = ".jpg" );

        LCubeTextureData* loadCubeTexture( const char* basename, const char* ftype = ".jpg" );
};



}