

#include "LLightPoint.h"
#include "LShaderManager.h"

using namespace std;

namespace engine
{

    int LLightPoint::s_count = 0;

    LLightPoint::LLightPoint( const LVec3& ambient,
                              const LVec3& diffuse,
                              const LVec3& specular,
                              int lIndx,
                              const LVec3& position,
                              float attenuation_const,
                              float attenuation_linear,
                              float attenuation_quadratic )
        : LILight( ambient, diffuse, specular, lIndx )
    {
        m_position = position;
        m_type = "positional";

        m_attenuation_const     = attenuation_const;
        m_attenuation_linear    = attenuation_linear;
        m_attenuation_quadratic = attenuation_quadratic;

        LLightPoint::s_count++;
    }

    LLightPoint::~LLightPoint()
    {
        LLightPoint::s_count--;
    }


    void LLightPoint::bind()
    {
        GLuint _currentShader = LShaderManager::INSTANCE->currentShader;

        // TODO: Do this string handling in a shader class
        string _u_location = "u_pointLights";
        _u_location += "[";
        _u_location += to_string( m_indx );
        _u_location += "]";

        string _u_variant = "";

        GLuint _u_ambient, _u_diffuse, _u_specular, _u_position, _u_active, _u_att_const, _u_att_lin, _u_att_quad;

        _u_variant = _u_location; _u_variant += ".ambient";
        _u_ambient = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant = _u_location; _u_variant += ".diffuse";
        _u_diffuse = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant  = _u_location; _u_variant += ".specular";
        _u_specular = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant   = _u_location; _u_variant += ".position";
        _u_position = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant   = _u_location; _u_variant += ".constant";
        _u_att_const = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant   = _u_location; _u_variant += ".linear";
        _u_att_lin = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant   = _u_location; _u_variant += ".quadratic";
        _u_att_quad = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant   = _u_location; _u_variant += ".isActive";
        _u_active = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        glUniform3f( _u_ambient, m_ambient.x, m_ambient.y, m_ambient.z );
        glUniform3f( _u_diffuse, m_diffuse.x, m_diffuse.y, m_diffuse.z );
        glUniform3f( _u_specular, m_specular.x, m_specular.y, m_specular.z );
        glUniform3f( _u_position, m_position.x, m_position.y, m_position.z );
        glUniform1f( _u_att_const, m_attenuation_const );
        glUniform1f( _u_att_lin, m_attenuation_linear );
        glUniform1f( _u_att_quad, m_attenuation_quadratic );
        glUniform1i( _u_active, active );

    }

    void LLightPoint::unbind()
    {
        
    }



}