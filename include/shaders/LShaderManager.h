

#pragma once

#include "../Common.h"
#include <shaders/LShader.h>

#include <glad/glad.h>
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
        map< string, LShader* > programObjs;

        static LShaderManager* INSTANCE;

        static void create();
        static void release();

        ~LShaderManager();

        GLuint createShader( const char* filename, int shaderType );
        GLuint createProgram( GLuint vShader, GLuint fShader );


    };

}