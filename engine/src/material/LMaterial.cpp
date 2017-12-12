

#include "../../include/material/LMaterial.h"
#include "../../include/shader/LShaderManager.h"


namespace engine
{

    LMaterial::LMaterial()
    {
        m_ambient    = MAT_DEFAULT_AMBIENT;
        m_diffuse    = MAT_DEFAULT_DIFFUSE;
        m_specular   = MAT_DEFAULT_SPECULAR;
        m_shininess  = MAT_DEFAULT_SHININESS;

        m_plainColor = MAT_DEFAULT_PLAIN;

        m_lightingEnabled = false;
    }

    LMaterial::LMaterial( const LVec3& cAmbient, 
                          const LVec3& cDiffuse,
                          const LVec3& cSpecular,
                          const LVec3& cPlainColor,
                          float cShininess )
    {
        m_ambient   = cAmbient;
        m_diffuse   = cDiffuse;
        m_specular  = cSpecular;
        m_shininess = cShininess;

        m_plainColor = cPlainColor;

        m_lightingEnabled = false;
    }

    LMaterial::~LMaterial()
    {
        for ( LTexture* tex : m_textures )
        {
            delete tex;
        }

        m_textures.clear();
    }

    void LMaterial::bind()
    {

        for ( LTexture* tex : m_textures )
        {
            tex->bind();
        }

        GLuint _currentShader = LShaderManager::INSTANCE->currentShader;
        if ( !m_lightingEnabled )
        {
            GLuint u_color = glGetUniformLocation( _currentShader, "u_color" );
            glUniform3f( u_color, m_plainColor.x, m_plainColor.y, m_plainColor.z );
        }
        else
        {
            GLuint u_material_ambient   = glGetUniformLocation( _currentShader, "u_material.ambient" );
            GLuint u_material_diffuse   = glGetUniformLocation( _currentShader, "u_material.diffuse" );
            GLuint u_material_specular  = glGetUniformLocation( _currentShader, "u_material.specular" );
            GLuint u_material_shininess = glGetUniformLocation( _currentShader, "u_material.shininess" );

            glUniform3f( u_material_ambient, m_ambient.x, m_ambient.y, m_ambient.z );
            glUniform3f( u_material_diffuse, m_diffuse.x, m_diffuse.y, m_diffuse.z );
            glUniform3f( u_material_specular, m_specular.x, m_specular.y, m_specular.z );
            glUniform1f( u_material_shininess, m_shininess );
        }

        for ( LTexture* tex : m_textures )
        {
            tex->unbind();
        }
    }



    void LMaterial::unbind()
    {
        // Unbind some texture stuff
    }

}