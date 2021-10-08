
#include <shaders/CShader.h>

namespace engine
{
    std::string ToString( const eShaderType& type )
    {
        /**/ if ( type == eShaderType::VERTEX ) return "vertex-shader";
        else if ( type == eShaderType::FRAGMENT ) return "fragment-shader";

        ENGINE_CORE_ASSERT( false, "Invalid eShaderType enum given" );
        return "undefined";
    }

    uint32 ToOpenGLEnum( const eShaderType& type )
    {
        /**/ if ( type == eShaderType::VERTEX ) return GL_VERTEX_SHADER;
        else if ( type == eShaderType::FRAGMENT ) return GL_FRAGMENT_SHADER;

        ENGINE_CORE_ASSERT( false, "Invalid eShaderType enum given" );
        return 0;
    }

    CShader::CShader( const std::string& name, const eShaderType& type, const std::string& source )
        : m_Name( name ), m_Type( type ), m_Source( source )
    {
        _CompileShaderFromSource();
    }

    CShader::~CShader()
    {
        glDeleteProgram( m_OpenglID );
        m_OpenglID = 0;
    }

    void CShader::_CompileShaderFromSource()
    {
        // Compile shader of given type
        const char* shader_source = m_Source.c_str();
        m_OpenglID = glCreateShader( ToOpenGLEnum( m_Type ) );
        glShaderSource( m_OpenglID, 1, &shader_source, NULL );
        glCompileShader( m_OpenglID );

        // Check for the status of the compiling process
        int32 compiling_success;
        glGetShaderiv( m_OpenglID, GL_COMPILE_STATUS, &compiling_success );
        if ( !compiling_success )
        {
            constexpr ssize_t ERROR_BUFFER_SIZE = 1024;
            GLchar error_log_buffer[ERROR_BUFFER_SIZE];
            glGetShaderInfoLog( m_OpenglID, ERROR_BUFFER_SIZE, NULL, error_log_buffer );
            ENGINE_CORE_ERROR( "CShader::_CompileShaderFromSource >>> Couldn't compile shader of type {0}: \n{1}",
                               engine::ToString( m_Type ), error_log_buffer );
            m_Compiled = false;
            m_OpenglID = 0;
            return;
        }
        m_Compiled = true;
    }
}
