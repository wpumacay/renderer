
#pragma once

#include "LCommon.h"

namespace engine
{


	class LFrameBuffer
	{

		protected :

		GLuint m_frameBufferId;
		GLuint m_frameBufferTex;

		GLint m_fboFormat;
		GLint m_fboDataType;
		GLint m_fboAttachmentType;

		int m_width;
		int m_height;

		public :

		LFrameBuffer( GLint fboFormat, GLint fboDataType, GLint fboAttachmentType, int width, int height );
		~LFrameBuffer();

		void bind();
		void unbind();

		int width() { return m_width; }
		int height() { return m_height; }

		GLuint getFBO() { return m_frameBufferId; }
		GLuint getTex() { return m_frameBufferTex; }

	};


}