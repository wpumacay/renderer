

#include "LLightSpot.h"
#include "LShaderManager.h"
#include <cmath>

using namespace std;

namespace engine
{

    int LLightSpot::s_count = 0;

    LLightSpot::LLightSpot( const LVec3& ambient,
                            const LVec3& diffuse,
                            const LVec3& specular,
                            int lIndx,
                            const LVec3& position,
                            const LVec3& direction,
                            float attenuation_const,
                            float attenuation_linear,
                            float attenuation_quadratic,
                            float cutOff, float outerCutOff )
        : LILight( ambient, diffuse, specular, lIndx )
    {
        m_position = position;
        m_direction = direction;
        m_type = "spot";

        m_attenuation_const     = attenuation_const;
        m_attenuation_linear    = attenuation_linear;
        m_attenuation_quadratic = attenuation_quadratic;

        m_cutOff      = cos( toRadians( cutOff ) );
        m_outerCutOff = cos( toRadians( outerCutOff ) );

        LLightSpot::s_count++;
    }

    LLightSpot::~LLightSpot()
    {
        LLightSpot::s_count--;
    }


    void LLightSpot::bind()
    {
        GLuint _currentShader = LShaderManager::INSTANCE->currentShader;

        // TODO: Do this string handling in a shader class
        string _u_location = "u_spotLights";
        _u_location += "[";
        _u_location += to_string( m_indx );
        _u_location += "]";

        string _u_variant = "";

        GLuint _u_ambient, _u_diffuse, _u_specular, _u_position, _u_direction, 
               _u_active, _u_att_const, _u_att_lin, _u_att_quad, _u_cutOff, _u_outerCutOff;

        _u_variant = _u_location; _u_variant += ".ambient";
        _u_ambient = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant = _u_location; _u_variant += ".diffuse";
        _u_diffuse = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant  = _u_location; _u_variant += ".specular";
        _u_specular = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant   = _u_location; _u_variant += ".position";
        _u_position = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant   = _u_location; _u_variant += ".direction";
        _u_direction = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant   = _u_location; _u_variant += ".constant";
        _u_att_const = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant   = _u_location; _u_variant += ".linear";
        _u_att_lin = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant   = _u_location; _u_variant += ".quadratic";
        _u_att_quad = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant   = _u_location; _u_variant += ".cutOff";
        _u_cutOff = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant   = _u_location; _u_variant += ".outerCutOff";
        _u_outerCutOff = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        _u_variant   = _u_location; _u_variant += ".isActive";
        _u_active = glGetUniformLocation( _currentShader, _u_variant.c_str() );

        glUniform3f( _u_ambient, m_ambient.x, m_ambient.y, m_ambient.z );
        glUniform3f( _u_diffuse, m_diffuse.x, m_diffuse.y, m_diffuse.z );
        glUniform3f( _u_specular, m_specular.x, m_specular.y, m_specular.z );
        glUniform3f( _u_position, m_position.x, m_position.y, m_position.z );
        glUniform3f( _u_direction, m_direction.x, m_direction.y, m_direction.z );
        glUniform1f( _u_att_const, m_attenuation_const );
        glUniform1f( _u_att_lin, m_attenuation_linear );
        glUniform1f( _u_att_quad, m_attenuation_quadratic );
        glUniform1f( _u_cutOff, m_cutOff );
        glUniform1f( _u_outerCutOff, m_outerCutOff );
        glUniform1i( _u_active, active );

    }

    void LLightSpot::unbind()
    {
        
    }



}