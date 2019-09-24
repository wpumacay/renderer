
#pragma once

#include "LLightDirectional.h"

#include <core/CFrameBuffer.h>

#define SHADOW_WIDTH 4096
#define SHADOW_HEIGHT 4096

namespace engine
{
    
    class LShadowMap
    {

        private :

        CFrameBuffer* m_framebuffer;

        CMat4 m_viewMat;
        CMat4 m_projMat;

        int m_width;
        int m_height;

        int m_prevViewWidth;
        int m_prevViewHeight;

        CVec3 m_virtualLightPosition;

        public :


        LShadowMap( int width = SHADOW_WIDTH, int height = SHADOW_HEIGHT );
        ~LShadowMap();

        void setupLightDirectional( LLightDirectional* pLight );

        void bind();
        void unbind();

        GLuint getDepthTexture() const;

        CMat4 getLightSpaceViewMatrix() { return m_viewMat; }
        CMat4 getLightSpaceProjectionMatrix() { return m_projMat; }
        CVec3 getLightPosition() { return m_virtualLightPosition; }
    };



}