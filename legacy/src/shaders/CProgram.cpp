
#include <shaders/CProgram.h>

namespace engine
{
    CProgram::CProgram( const std::string& name,
                        const std::string& vertexShaderSource,
                        const std::string& fragmentShaderSource )
        : m_Name( name ), m_OpenglID( 0 )
    {
        auto vertex_shader = std::make_unique<CShader>( name + "_vert", eShaderType::VERTEX, vertexShaderSource );
        auto fragment_shader = std::make_unique<CShader>( name + "_frag", eShaderType::FRAGMENT, fragmentShaderSource );
        _LinkShaderProgram( std::move( vertex_shader ), std::move( fragment_shader ) );
    }

    CProgram::~CProgram()
    {
        m_VertexShader = nullptr;
        m_FragmentShader = nullptr;
        glDeleteProgram( m_OpenglID );
    }

    void CProgram::Bind()
    {
        glUseProgram( m_OpenglID );
    }

    void CProgram::Unbind()
    {
        glUseProgram( 0 );
    }

    void CProgram::SetInt( const std::string& uniform_name, const int32& uniform_value )
    {
        glUniform1i( _GetUniformLocation( uniform_name ), uniform_value );
    }

    void CProgram::SetFloat( const std::string& uniform_name, const float32& uniform_value )
    {
        glUniform1f( _GetUniformLocation( uniform_name ), uniform_value );
    }

    void CProgram::SetVec2( const std::string& uniform_name, const CVec2& uniform_value )
    {
        glUniform2fv( _GetUniformLocation( uniform_name ), 1, uniform_value.data() );
    }

    void CProgram::SetVec3( const std::string& uniform_name, const CVec3& uniform_value )
    {
        glUniform3fv( _GetUniformLocation( uniform_name ), 1, uniform_value.data() );
    }

    void CProgram::SetVec4( const std::string& uniform_name, const CVec4& uniform_value )
    {
        glUniform4fv( _GetUniformLocation( uniform_name ), 1, uniform_value.data() );
    }

    void CProgram::SetMat4( const std::string& uniform_name, const CMat4& uniform_value )
    {
        glUniformMatrix4fv( _GetUniformLocation( uniform_name ), 1, GL_FALSE, uniform_value.data() );
    }

    std::string CProgram::GetVertexShaderSource() const
    {
        if ( m_VertexShader )
            return m_VertexShader->source();
        return "";
    }

    std::string CProgram::GetFragmentShaderSource() const
    {
        if ( m_FragmentShader )
            return m_FragmentShader->source();
        return "";
    }

    void CProgram::_LinkShaderProgram( std::unique_ptr<CShader> vertex_shader, std::unique_ptr<CShader> fragment_shader )
    {
        if ( !vertex_shader->compiled() || !fragment_shader->compiled() )
        {
            ENGINE_CORE_ERROR( "CProgram::_LinkShaderProgram >>> given shaders {0} and {1} are not compiled correctly",
                               vertex_shader->name(), fragment_shader->name() );
            return;
        }

        m_OpenglID = glCreateProgram();
        const uint32 vertShaderId = vertex_shader->opengl_id();
        const uint32 fragShaderId = fragment_shader->opengl_id();
        glAttachShader( m_OpenglID, vertShaderId );
        glAttachShader( m_OpenglID, fragShaderId );
        glLinkProgram( m_OpenglID );
        glDetachShader( m_OpenglID, vertShaderId );
        glDetachShader( m_OpenglID, fragShaderId );
        glDeleteShader( vertShaderId );
        glDeleteShader( fragShaderId );

        // Check for the status of the linking process
        int32 linking_success;
        glGetProgramiv( m_OpenglID, GL_LINK_STATUS, &linking_success );
        if ( !linking_success )
        {
            constexpr ssize_t ERROR_BUFFER_SIZE = 1024;
            GLchar error_log_buffer[ERROR_BUFFER_SIZE];
            glGetProgramInfoLog( m_OpenglID, ERROR_BUFFER_SIZE, NULL, error_log_buffer );
            glDeleteProgram( m_OpenglID );
            ENGINE_CORE_ERROR( "CProgram::_LinkShaderProgram >>> Couldn't link shader-program : \n{0}", error_log_buffer );
            m_VertexShader = nullptr;
            m_FragmentShader = nullptr;
            m_OpenglID = 0;
            return;
        }

        m_VertexShader = std::move( vertex_shader );
        m_FragmentShader = std::move( fragment_shader );
    }

    uint32 CProgram::_GetUniformLocation( const std::string& uniform_name )
    {
        if ( m_UniformsLocations.find( uniform_name ) == m_UniformsLocations.end() )
            m_UniformsLocations[uniform_name] = glGetUniformLocation( m_OpenglID, uniform_name.c_str() );

        if ( m_UniformsLocations[uniform_name] == -1 )
            ENGINE_CORE_ERROR( "CProgram::_GetUniformLocation >>> Uniform location '{0}' was not found in shader '{1}'", uniform_name, m_Name );

        return m_UniformsLocations[uniform_name];
    }
}
