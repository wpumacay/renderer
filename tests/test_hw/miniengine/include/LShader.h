
#pragma once

#include "../LMiniCommon.h"
#include <fstream>
#include <sstream>

using namespace std;

#define ERRORLOG_BUFF_LENGTH 512

namespace miniengine
{


        class LShader
        {

        private :

            GLuint m_type;
            LShader( GLuint pType, const GLchar* pShaderCode );

        public :

            GLuint id;

            static LShader createFromFile( GLuint pType, const GLchar* pPath );
            ~LShader();
            GLuint type();

        };

        class LProgram
        {

        public :

            static LProgram createProgram( const GLchar* pVertexShaderResPath,
                                           const GLchar* pFragmentShaderResPath );

            LProgram( const vector<LShader>& pShaders );
            ~LProgram();
            GLuint id;
        };








}