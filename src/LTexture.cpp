

#include <LTexture.h>


namespace engine
{

    LTexture::LTexture( const LTextureData& textureData )
    {
        glGenTextures( 1, &m_textureId );

        m_textureData = textureData;
        m_textureIndx = 0;

        glBindTexture( GL_TEXTURE_2D, m_textureId );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        glTexImage2D( GL_TEXTURE_2D, 0, 
                      m_textureData.ftype, m_textureData.width, m_textureData.height, 0, 
                      m_textureData.ftype, GL_UNSIGNED_BYTE, m_textureData.data );
        glGenerateMipmap( GL_TEXTURE_2D );
    }

    LTexture::~LTexture()
    {
        glDeleteTextures( 1, &m_textureId );
    }

    void LTexture::bind()
    {
        glActiveTexture( GL_TEXTURE0 + m_textureIndx );
        glBindTexture( GL_TEXTURE_2D, m_textureId );
    }

    void LTexture::unbind()
    {
        glBindTexture( GL_TEXTURE_2D, 0 );
    }
}