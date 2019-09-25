
#include <shaders/CShader.h>

namespace engine
{

    CShader::CShader( const std::string& name, uint32 openglId )
    {
        m_name      = name;
        m_openglId  = openglId;
    }

    CShader::~CShader()
    {
        glDeleteProgram( m_openglId );
    }

    void CShader::bind()
    {
        glUseProgram( m_openglId );
    }

    void CShader::unbind()
    {
        glUseProgram( 0 );
    }

    void CShader::setInt( const std::string& uName, int32 value )
    {
        glUniform1i( _getUniformLocation( uName ), value );
    }

    void CShader::setFloat( const std::string& uName, float32 value )
    {
        glUniform1f( _getUniformLocation( uName ), value );
    }

    void CShader::setVec2( const std::string& uName, const CVec2& vec )
    {
        glUniform2fv( _getUniformLocation( uName ), 1, ( float32* ) &vec );
    }

    void CShader::setVec3( const std::string& uName, const CVec3& vec )
    {
        glUniform3fv( _getUniformLocation( uName ), 1, ( float32* ) &vec );
    }

    void CShader::setVec4( const std::string& uName, const CVec4& vec )
    {
        glUniform4fv( _getUniformLocation( uName ), 1, ( float32* ) &vec );
    }

    void CShader::setMat4( const std::string& uName, const CMat4& mat )
    {
        glUniformMatrix4fv( _getUniformLocation( uName ), 1, GL_FALSE, mat.buff );
    }

    void CShader::_setInt( uint32 uniform, int32 value )
    {
        glUniform1i( uniform, value );
    }

    void CShader::_setFloat( uint32 uniform, float32 value )
    {
        glUniform1f( uniform, value );        
    }

    void CShader::_setVec2( uint32 uniform, const CVec2& vec )
    {
        glUniform2fv( uniform, 1, ( float32* ) &vec );
    }

    void CShader::_setVec3( uint32 uniform, const CVec3& vec )
    {
        glUniform3fv( uniform, 1, ( float32* ) &vec );
    }

    void CShader::_setVec4( uint32 uniform, const CVec4& vec )
    {
        glUniform4fv( uniform, 1, ( float32* ) &vec );
    }

    void CShader::_setMat4( uint32 uniform, const CMat4& mat )
    {
        glUniformMatrix4fv( uniform, 1, GL_FALSE, mat.buff );
    }

    uint32 CShader::_getUniformLocation( const std::string& uniformName )
    {
        if ( m_uniformLocations.find( uniformName ) == m_uniformLocations.end() )
            m_uniformLocations[uniformName] = glGetUniformLocation( m_openglId, uniformName.c_str() );

        if ( m_uniformLocations[uniformName] == -1 )
            ENGINE_CORE_ERROR( "Uniform location '{0}' was not found in shader '{1}'", uniformName, m_name );

        return m_uniformLocations[uniformName];
    }

}