#pragma once

#include <CCommon.h>
#include <CMath.h>

namespace engine
{

    struct CCubeTextureData
    {
        u8* vpData[6];
        int width;
        int height;
        int channels;
        int ftype;
    };

    class LCubeTexture
    {

        private :

        GLuint m_cubeTextureId;
        GLuint m_cubeTextureIndx;
        CCubeTextureData m_cubeTextureData;

        public :

        LCubeTexture( const CCubeTextureData& cubeTextureData );
        ~LCubeTexture();

        void bind();
        void unbind();

        CCubeTextureData data() { return m_cubeTextureData; }
        
        void setTextureIndex( GLuint cubeTextureIndx ) { m_cubeTextureIndx = cubeTextureIndx; }
        GLuint getTextureIndex() { return m_cubeTextureIndx; }

        void log()
        {
            std::cout << "CUBEMAP ********************" << std::endl;
            std::cout << "width: " << m_cubeTextureData.width << std::endl;
            std::cout << "height: " << m_cubeTextureData.height << std::endl;
            std::cout << "channels: " << m_cubeTextureData.channels << std::endl;
            std::cout << "ftype: " << m_cubeTextureData.ftype << std::endl;
            std::cout << "textureindx: " << m_cubeTextureIndx << std::endl;
            std::cout << "****************************" << std::endl;
        }
    };




}