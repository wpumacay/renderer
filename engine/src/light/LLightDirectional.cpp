

#include "LLightDirectional.h"
#include "LShaderManager.h"

using namespace std;

namespace engine
{

    int LLightDirectional::s_count = 0;

    LLightDirectional::LLightDirectional( const LVec3& ambient,
                                          const LVec3& diffuse,
                                          const LVec3& specular,
                                          int lIndx,
                                          const LVec3& direction )
        : LILight( ambient, diffuse, specular, lIndx )
    {
        m_direction = direction;
        m_type = "directional";

        LLightDirectional::s_count++;
    }

    LLightDirectional::~LLightDirectional()
    {
        LLightDirectional::s_count--;
    }


    void LLightDirectional::bind()
    {
        GLuint _currentShader = LShaderManager::INSTANCE->currentShader;

        // TODO: Do this string handling in a shader class
        string _u_location = "u_directionalLights";
        _u_location += "[";
        _u_location += to_string( m_indx );
        _u_location += "]";

        string _u_variant = "";

        GLuint _u_ambient, _u_diffuse, _u_specular, _u_direction, _u_active;

        _u_variant = _u_location; _u_variant += ".ambient";
        _u_ambient = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant = _u_location; _u_variant += ".diffuse";
        _u_diffuse = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant  = _u_location; _u_variant += ".specular";
        _u_specular = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant   = _u_location; _u_variant += ".direction";
        _u_direction = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant   = _u_location; _u_variant += ".isActive";
        _u_active = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        glUniform3f( _u_ambient, m_ambient.x, m_ambient.y, m_ambient.z );
        glUniform3f( _u_diffuse, m_diffuse.x, m_diffuse.y, m_diffuse.z );
        glUniform3f( _u_specular, m_specular.x, m_specular.y, m_specular.z );
        glUniform3f( _u_direction, m_direction.x, m_direction.y, m_direction.z );
        glUniform1i( _u_active, active );

    }

    void LLightDirectional::unbind()
    {
        
    }



}