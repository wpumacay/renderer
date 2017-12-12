

#pragma once

#include "../LCommon.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

namespace engine
{

    struct LShaderManager
    {

        private :

        LShaderManager();

        public :

        map< string, GLuint > programs;
        GLuint currentShader;

        static LShaderManager* INSTANCE;

        static void create();
        ~LShaderManager();
        void release();

        void setCurrentShader( string sId );

        GLuint createShader( string strFilename, int shaderType );
        GLuint createProgram( GLuint vShader, GLuint fShader );


    };

}