
#include "LCubeTexture.h"



namespace engine
{

    LCubeTexture::LCubeTexture()
    {
        glGenTextures( 1, &m_textureId );
    }

    LCubeTexture::~LCubeTexture()
    {
        glDeleteTextures( 1, &m_textureId );
    }


    void LCubeTexture::setData( LCubeTextureData* pCubeData, GLuint type, GLuint textureIndx )
    {
        m_width = pCubeData->width;
        m_height = pCubeData->height;
        m_textureIndx = textureIndx;

        glBindTexture( GL_TEXTURE_CUBE_MAP, m_textureId );

        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );       

        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        for ( int q = 0; q < 6; q++ )
        {
            glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + q, 0, type, 
                          m_width, m_height, 0, type, GL_UNSIGNED_BYTE, pCubeData->vpData[q] );
        }


    }

    void LCubeTexture::bind()
    {
        glActiveTexture( GL_TEXTURE0 + m_textureIndx );
        glBindTexture( GL_TEXTURE_CUBE_MAP, m_textureId );
    }

    void LCubeTexture::unbind()
    {
        //glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
    }
}