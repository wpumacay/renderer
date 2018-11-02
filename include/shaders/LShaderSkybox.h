
#pragma once

#include <shaders/LShader.h>

namespace engine
{


    class LShaderSkybox : public LShader
    {
        protected :

        GLuint m_uView;
        GLuint m_uProj;

        public :

        LShaderSkybox( GLuint id ) : LShader( id )
        {
            bind();

            // Load uniforms
            m_uProj = glGetUniformLocation( m_id, "u_tProj" );
            m_uView  = glGetUniformLocation( m_id, "u_tView" );

            unbind();
        }

        void setViewMatrix( const LMat4& mat )
        {
            _setMat4( m_uView, mat.extractRotation() );
        }

        void setProjectionMatrix( const LMat4& mat )
        {
            _setMat4( m_uProj, mat );
        }

    };



}