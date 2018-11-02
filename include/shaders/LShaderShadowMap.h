
#pragma once

#include <shaders/LShader.h>

namespace engine
{

    class LShaderShadowMap : public LShader
    {

        private :

        GLuint m_uModelMatrix;
        GLuint m_uLightSpaceViewMatrix;
        GLuint m_uLightSpaceProjMatrix;

        public :

        LShaderShadowMap( GLuint id ) : LShader( id )
        {
            bind();

            m_uModelMatrix           = glGetUniformLocation( m_id, "u_modelMatrix" );
            m_uLightSpaceViewMatrix  = glGetUniformLocation( m_id, "u_lightSpaceViewMatrix" );
            m_uLightSpaceProjMatrix  = glGetUniformLocation( m_id, "u_lightSpaceProjMatrix" );

            unbind();
        }

        void setModelMatrix( const LMat4& mat )
        {
            _setMat4( m_uModelMatrix, mat );
        }

        void setLightSpaceViewMatrix( const LMat4& mat )
        {
            _setMat4( m_uLightSpaceViewMatrix, mat );
        }

        void setLightSpaceProjectionMatrix( const LMat4& mat )
        {
            _setMat4( m_uLightSpaceProjMatrix, mat );
        }

    };


}