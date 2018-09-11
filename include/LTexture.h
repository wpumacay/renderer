

#pragma once

#include "LCommon.h"

namespace engine
{

    class LTexture
    {

        private :

        GLuint m_textureId;
        GLuint m_textureIndx;
        int m_width;
        int m_height;

        public :

        bool enabled;

        LTexture();
        ~LTexture();

        void setData( u8* pData, GLuint type, int width, int height, GLuint textureIndx );
        void bind();
        void unbind();

        int width() { return m_width; }
        int height() { return m_height; }

        GLuint getTextureIndx() { return m_textureIndx; }

    };




}