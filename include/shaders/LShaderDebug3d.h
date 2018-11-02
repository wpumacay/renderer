
#pragma once

#include <shaders/LShader.h>

namespace engine
{


    class LShaderDebug3d : public LShader
    {
        protected :

        GLuint m_uView;
        GLuint m_uProj;

        public :

        LShaderDebug3d( GLuint id ) : LShader( id )
        {
            bind();

            // Load uniforms
            m_uView  = glGetUniformLocation( m_id, "u_tView" );
            m_uProj  = glGetUniformLocation( m_id, "u_tProj" );

            unbind();
        }

        void setViewMatrix( const LMat4& mat )
        {
            _setMat4( m_uView, mat );
        }

        void setProjectionMatrix( const LMat4& mat )
        {
            _setMat4( m_uProj, mat );
        }

    };



}