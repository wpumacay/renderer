
#pragma once

#include <shaders/LShader.h>

namespace engine
{


    class LShaderEnvMapping : public LShader
    {
        protected :

        GLuint m_uModel;
        GLuint m_uView;
        GLuint m_uProj;
        GLuint m_uViewPos;

        public :

        LShaderEnvMapping( GLuint id ) : LShader( id )
        {
            bind();

            // Load uniforms
            m_uModel    = glGetUniformLocation( m_id, "u_tModel" );
            m_uView     = glGetUniformLocation( m_id, "u_tView" );
            m_uProj     = glGetUniformLocation( m_id, "u_tProj" );
            m_uViewPos  = glGetUniformLocation( m_id, "u_viewPos" );

            unbind();
        }

        void setModelMatrix( const LMat4& mat )
        {
            _setMat4( m_uModel, mat );
        }

        void setViewMatrix( const LMat4& mat )
        {
            _setMat4( m_uView, mat );
        }

        void setProjectionMatrix( const LMat4& mat )
        {
            _setMat4( m_uProj, mat );
        }

        void setViewPosition( const LVec3& viewPos )
        {
            _setVec3( m_uViewPos, viewPos );
        }

    };



}