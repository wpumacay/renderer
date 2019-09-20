
#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <shaders/CShaderUniforms.h>

using namespace std;

namespace engine
{


    class LShader
    {

        protected :

        GLuint m_id;

        void _setInt( GLuint uniform, GLint value );
        void _setFloat( GLuint uniform, GLfloat value );
        void _setVec2( GLuint uniform, const CVec2& vec );
        void _setVec3( GLuint uniform, const CVec3& vec );
        void _setVec4( GLuint uniform, const CVec4& vec );
        void _setMat4( GLuint uniform, const CMat4& mat );

        public :

        LShader( GLuint id );
        ~LShader();

        GLuint id() { return m_id; }

        void bind();
        void unbind();

        void setInt( const string& uName, GLint value );
        void setFloat( const string& uName, GLfloat value );
        void setVec2( const string& uName, const CVec2& vec );
        void setVec3( const string& uName, const CVec3& vec );
        void setVec4( const string& uName, const CVec4& vec );
        void setMat4( const string& uName, const CMat4& mat );


    };


}