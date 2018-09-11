
#include "LFrameBuffer.h"

using namespace std;

namespace engine
{

    LFrameBuffer::LFrameBuffer( GLint fboFormat, 
                                GLint fboDataType, 
                                GLint fboAttachmentType, 
                                int width, int height )
    {
        m_fboFormat = fboFormat;
        m_fboDataType = fboDataType;
        m_fboAttachmentType = fboAttachmentType;

        m_width = width;
        m_height = height;

        // create framebuffer
        glGenFramebuffers( 1, &m_frameBufferId );
        glBindFramebuffer( GL_FRAMEBUFFER, m_frameBufferId );
        // create a texture attachment
        glGenTextures( 1, &m_frameBufferTex );
        glBindTexture( GL_TEXTURE_2D, m_frameBufferTex );
        glTexImage2D( GL_TEXTURE_2D, 0, fboFormat, 
                      m_width, m_height, 0, m_fboFormat,
                      m_fboDataType, NULL );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glBindTexture( GL_TEXTURE_2D, 0 );
        // Attach to current frame buffer
        glFramebufferTexture2D( GL_FRAMEBUFFER, m_fboAttachmentType, GL_TEXTURE_2D, m_frameBufferTex, 0 );

        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
        
    }

    LFrameBuffer::~LFrameBuffer()
    {
        glDeleteFramebuffers( 1, &m_frameBufferId );
        glDeleteTextures( 1, &m_frameBufferTex );
    }

    void LFrameBuffer::bind()
    {
        glBindFramebuffer( GL_FRAMEBUFFER, m_frameBufferId );
    }

    void LFrameBuffer::unbind()
    {
        glBindFramebuffer( GL_FRAMEBUFFER, 0 );
    }

}