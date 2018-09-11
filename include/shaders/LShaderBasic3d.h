
#pragma once

#include <shaders/LShader.h>

namespace engine
{


    class LShaderBasic3d : public LShader
    {
        protected :

        GLuint m_uModel;
        GLuint m_uView;
        GLuint m_uProj;
        GLuint m_uColor;

        public :

        LShaderBasic3d( GLuint id ) : LShader( id )
        {
            bind();

            // Load uniforms
            m_uModel = glGetUniformLocation( m_id, "u_tModel" );
            m_uView  = glGetUniformLocation( m_id, "u_tView" );
            m_uProj  = glGetUniformLocation( m_id, "u_tProj" );
            m_uColor = glGetUniformLocation( m_id, "u_color" );

            unbind();
        }

        void setModelMatrix( const glm::mat4& mat )
        {
            _setMat4( m_uModel, mat );
        }

        void setViewMatrix( const glm::mat4& mat )
        {
            _setMat4( m_uView, mat );
        }

        void setProjectionMatrix( const glm::mat4& mat )
        {
            _setMat4( m_uProj, mat );
        }

        void setColor( const LVec3& color )
        {
            _setVec3( m_uColor, color );
        }

    };



}