#pragma once

#include "LCommon.h"

namespace engine
{

    class LCubeTexture
    {

        private :

        GLuint m_textureId;
        GLuint m_textureIndx;
        int m_width;
        int m_height;

        public :

        LCubeTexture();
        ~LCubeTexture();

        void setData( LCubeTextureData* pCubeData, GLuint type, GLuint textureIndx );
        void bind();
        void unbind();

        int width() { return m_width; }
        int height() { return m_height; }

    };




}