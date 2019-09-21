#pragma once

#include <CCommon.h>
#include <CMath.h>

namespace engine
{

    struct CTextureData
    {
        uint8*  data;
        int     width;
        int     height;
        int     channels;
        int     ftype;
    };

    class LTexture
    {

        private :

        GLuint m_textureId;
        GLuint m_textureIndx;
        CTextureData m_textureData;

        public :

        LTexture( const CTextureData& textureData );
        ~LTexture();

        void bind();
        void unbind();

        CTextureData data() { return m_textureData; }

        void setTextureIndex( GLuint textureIndx ) { m_textureIndx = textureIndx; }
        GLuint getTextureIndex() { return m_textureIndx; }

    };




}