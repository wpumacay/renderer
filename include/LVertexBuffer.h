

#pragma once

#include <LCommon.h>


namespace engine
{

    // For now, just single vertex buffer per attribute
    // Can be generalized to handle one buffer with different ...
    // attributes and different layouts

    class LVertexBuffer
    {

        private :

        GLuint m_bufferId;
        GLuint m_usage;
        GLuint m_componentCount;

        public :


        LVertexBuffer();
        LVertexBuffer( GLuint usage );

        ~LVertexBuffer();

        void setData( GLuint size, GLuint componentCount, GLfloat* pData );
        void updateData( GLuint size, GLfloat* pData );
        void bind();
        void unbind();

        GLuint getComponentCount() { return m_componentCount; }
    };



}