
#pragma once

#include <LCommon.h>


using namespace std;

namespace engine
{


    class LShader
    {

        protected :

        GLuint m_id;

        void _setInt( GLuint uniform, GLint value );
        void _setFloat( GLuint uniform, GLfloat value );
        void _setVec2( GLuint uniform, const LVec2& vec );
        void _setVec3( GLuint uniform, const LVec3& vec );
        void _setVec4( GLuint uniform, const LVec4& vec );
        void _setMat4( GLuint uniform, const LMat4& mat );

        public :

        LShader( GLuint id );
        ~LShader();

        GLuint id() { return m_id; }

        void bind();
        void unbind();

        void setInt( const string& uName, GLint value );
        void setFloat( const string& uName, GLfloat value );
        void setVec2( const string& uName, const LVec2& vec );
        void setVec3( const string& uName, const LVec3& vec );
        void setVec4( const string& uName, const LVec4& vec );
        void setMat4( const string& uName, const LMat4& mat );


    };


}