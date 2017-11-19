
#include "../include/LShader.h"

using namespace std;

namespace miniengine
{



    LShader::LShader( GLuint pType, const GLchar* pShaderCode )
    {
        m_type = pType;
        id = glCreateShader( m_type );
        glShaderSource( id, 1, &pShaderCode, NULL );
        glCompileShader( id );
        GLint _success;
        GLchar _infoLog[ERRORLOG_BUFF_LENGTH];
        glGetShaderiv( id, GL_COMPILE_STATUS, &_success );

        if ( !_success )
        {
            glGetShaderInfoLog( id, ERRORLOG_BUFF_LENGTH, NULL, _infoLog );
            cout << "LShader::LShader> error compiling " 
                 << ( m_type == GL_VERTEX_SHADER ? "vertex" : 
                        ( m_type == GL_FRAGMENT_SHADER ? "fragment" : "unknown" ) )
                 <<" LShader" << endl;
            cout << "COMPILATION_ERROR: " << _infoLog << endl;
        }


    }

    LShader::~LShader()
    {

    }

    LShader LShader::createFromFile( GLuint pType, const GLchar* pPath )
    {
        string _shaderStrCode;
        ifstream _shaderFile;

        _shaderFile.exceptions( ifstream::badbit );

        try 
        {
            _shaderFile.open( pPath );
            stringstream _shaderStream;
            _shaderStream << _shaderFile.rdbuf();

            _shaderFile.close();

            _shaderStrCode = _shaderStream.str();
        }
        catch ( ... )
        {
            cout << "LShader::createFromFile> failed opening the resource file" << endl;
        }

        const GLchar* _shaderCode_cstr = _shaderStrCode.c_str();

        return LShader( pType, _shaderCode_cstr );
    }



    GLuint LShader::type()
    {
        return m_type;
    }


    LProgram::LProgram( const vector<LShader> &pShaders )
    {
        id = glCreateProgram();
        for ( unsigned int i = 0; i < pShaders.size(); i++ )
        {
            glAttachShader( id, pShaders[i].id );
        }
        glLinkProgram( id );

        for( unsigned int i = 0; i < pShaders.size(); i++ )
        {
            glDetachShader( id, pShaders[i].id );
            glDeleteShader( pShaders[i].id );
        }

        GLint _success;
        GLchar _infoLog[ERRORLOG_BUFF_LENGTH];

        glGetProgramiv( id, GL_LINK_STATUS, &_success );
        if ( !_success )
        {
            glGetProgramInfoLog( id, ERRORLOG_BUFF_LENGTH, NULL, _infoLog );
            cout << "LINKING ERROR: " << _infoLog << endl;
        }
    }

    LProgram::~LProgram()
    {

    }

    LProgram LProgram::createProgram( const GLchar* pVertexShaderResPath,
                                      const GLchar* pFragmentShaderResPath )
    {

        vector<LShader> _shaders;
        _shaders.push_back( LShader::createFromFile( GL_VERTEX_SHADER, pVertexShaderResPath ) );
        _shaders.push_back( LShader::createFromFile( GL_FRAGMENT_SHADER, pFragmentShaderResPath ) );

        LProgram _res_program = LProgram( _shaders );

        return _res_program;
    }




}



