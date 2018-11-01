

#pragma once

#include <LCommon.h>

namespace engine
{

    class LIndexBuffer
    {

        private :

        GLuint m_bufferId;
        GLuint m_usage;
        GLuint m_size;
        GLuint m_count;

        public :

        LIndexBuffer();
        LIndexBuffer( GLuint usage );

        ~LIndexBuffer();

        void setData( GLuint size, GLuint count, GLuint* pData );
        void bind();
        void unbind();

        GLuint getCount() { return m_count; }

    };



}