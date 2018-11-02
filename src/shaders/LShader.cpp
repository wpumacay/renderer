

#include <shaders/LShader.h>


namespace engine
{


    LShader::LShader( GLuint id )
    {
        m_id = id;
    }

    LShader::~LShader()
    {
        glDeleteProgram( m_id );
        m_id = 0;
    }

    void LShader::bind()
    {
        glUseProgram( m_id );
    }

    void LShader::unbind()
    {
        glUseProgram( 0 );
    }

    void LShader::setInt( const string& uName, GLint value )
    {
        glUniform1i( glGetUniformLocation( m_id, uName.c_str() ), value );
    }

    void LShader::setFloat( const string& uName, GLfloat value )
    {
        glUniform1f( glGetUniformLocation( m_id, uName.c_str() ), value );
    }

    void LShader::setVec2( const string& uName, const LVec2& vec )
    {
        glUniform2fv( glGetUniformLocation( m_id, uName.c_str() ), 1, ( GLfloat* ) &vec );
    }

    void LShader::setVec3( const string& uName, const LVec3& vec )
    {
        glUniform3fv( glGetUniformLocation( m_id, uName.c_str() ), 1, ( GLfloat* ) &vec );
    }

    void LShader::setVec4( const string& uName, const LVec4& vec )
    {
        glUniform4fv( glGetUniformLocation( m_id, uName.c_str() ), 1, ( GLfloat* ) &vec );
    }

    void LShader::setMat4( const string& uName, const LMat4& mat )
    {
        glUniformMatrix4fv( glGetUniformLocation( m_id, uName.c_str() ), 1, GL_FALSE, mat.buff );
    }


    void LShader::_setInt( GLuint uniform, GLint value )
    {
        glUniform1i( uniform, value );
    }

    void LShader::_setFloat( GLuint uniform, GLfloat value )
    {
        glUniform1f( uniform, value );        
    }

    void LShader::_setVec2( GLuint uniform, const LVec2& vec )
    {
        glUniform2fv( uniform, 1, ( GLfloat* ) &vec );
    }

    void LShader::_setVec3( GLuint uniform, const LVec3& vec )
    {
        glUniform3fv( uniform, 1, ( GLfloat* ) &vec );
    }

    void LShader::_setVec4( GLuint uniform, const LVec4& vec )
    {
        glUniform4fv( uniform, 1, ( GLfloat* ) &vec );
    }

    void LShader::_setMat4( GLuint uniform, const LMat4& mat )
    {
        glUniformMatrix4fv( uniform, 1, GL_FALSE, mat.buff );
    }


}