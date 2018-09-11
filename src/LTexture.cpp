

#include "LTexture.h"



namespace engine
{

    LTexture::LTexture()
    {
        this->enabled = true;
        glGenTextures( 1, &m_textureId );
    }

    LTexture::~LTexture()
    {
        glDeleteTextures( 1, &m_textureId );
    }


    void LTexture::setData( u8* pData, GLuint type, int width, int height, GLuint textureIndx )
    {
        //cout << "width: " << width << endl;
        //cout << "height: " << height << endl;
        //cout << "textureIndx: " << textureIndx << endl;
        //if ( pData == NULL )
        //{
        //    cout << "??" << endl;
        //}

        m_width = width;
        m_height = height;
        m_textureIndx = textureIndx;

        glBindTexture( GL_TEXTURE_2D, m_textureId );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        glTexImage2D( GL_TEXTURE_2D, 0, type, m_width, m_height, 0, type, GL_UNSIGNED_BYTE, pData );
        glGenerateMipmap( GL_TEXTURE_2D );
    }

    void LTexture::bind()
    {
        glActiveTexture( GL_TEXTURE0 + m_textureIndx );
        glBindTexture( GL_TEXTURE_2D, m_textureId );
    }

    void LTexture::unbind()
    {
        //glBindTexture( GL_TEXTURE_2D, 0 );
    }
}