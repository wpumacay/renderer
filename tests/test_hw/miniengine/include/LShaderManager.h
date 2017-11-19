

#pragma once

#include "LShader.h"


namespace miniengine
{

    namespace shaders
    {
        enum
        {
            BASE_SHADER_3D_NO_LIGHTING,
            BASE_SHADER_3D_WITH_LIGHTING
        };
    }


    class LShaderManager
    {

        private :

        std::vector<LProgram> m_programs;

        LShaderManager();

        public :

        static LShaderManager* instance;
        static void create();
        ~LShaderManager();

        void initialize();

        GLuint createProgram( const GLchar* pVertexShaderResPath,
                              const GLchar* pFragmentShaderResPath );

        GLuint addProgram( LProgram pProgram );
        LProgram& getProgram( GLuint pId );

        GLuint shaderTable[100];
    };


}