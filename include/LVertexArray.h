

#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <LVertexBuffer.h>

using namespace std;

namespace engine
{


    class LVertexArray
    {

        private :

        GLuint m_vertexArrayId;

        vector<LVertexBuffer*> m_vBuffers;

        public :

        LVertexArray();
        ~LVertexArray();

        void addBuffer( LVertexBuffer* pBuffer, GLuint attribIndex );
        void bind();
        void unbind();


    };





}