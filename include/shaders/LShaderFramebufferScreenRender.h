
#pragma once

#include <shaders/LShader.h>
#include <shaders/CShaderUniforms.h>

namespace engine
{


    class LShaderFramebufferScreenRender : public LShader
    {
        protected :

        GLuint m_uTextureChannel;

        public :

        LShaderFramebufferScreenRender( GLuint id ) : LShader( id )
        {
            bind();

            // Load uniforms
            m_uTextureChannel = glGetUniformLocation( m_id, "u_texChannel" );

            unbind();
        }

        void setTextureSamplingChannel( int channel )
        {
            _setInt( m_uTextureChannel, channel );
        }

    };



}