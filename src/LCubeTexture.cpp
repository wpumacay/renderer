
#include "LCubeTexture.h"



namespace engine
{

    LCubeTexture::LCubeTexture( const LCubeTextureData& cubeTextureData )
    {
        glGenTextures( 1, &m_cubeTextureId );

        m_cubeTextureData = cubeTextureData;
        m_cubeTextureIndx = 0;

        glBindTexture( GL_TEXTURE_CUBE_MAP, m_cubeTextureId );

        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

        for ( int q = 0; q < 6; q++ )
        {
            glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + q, 0, 
                          m_cubeTextureData.ftype, m_cubeTextureData.width, m_cubeTextureData.height, 0, 
                          m_cubeTextureData.ftype, GL_UNSIGNED_BYTE, m_cubeTextureData.vpData[q] );
        }
    }

    LCubeTexture::~LCubeTexture()
    {
        glDeleteTextures( 1, &m_cubeTextureId );
    }

    void LCubeTexture::bind()
    {
        glActiveTexture( GL_TEXTURE0 + m_cubeTextureIndx );
        glBindTexture( GL_TEXTURE_CUBE_MAP, m_cubeTextureId );
    }

    void LCubeTexture::unbind()
    {
        // glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
    }
}