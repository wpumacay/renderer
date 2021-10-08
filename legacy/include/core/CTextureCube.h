#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <core/CTexture.h>

namespace engine
{

    struct CTextureCubeData
    {
        std::string                 name;
        std::array< uint8*, 6 >     sidesData;
        int                         width;
        int                         height;
        int                         channels;
        eTextureFormat              format;

        CTextureCubeData();
        ~CTextureCubeData();
    };

    std::string toString( const CTextureCubeData& texCubeData );

    class CTextureCube
    {

    public :

        CTextureCube( std::unique_ptr<CTextureCubeData> texCubeData );

        ~CTextureCube();

        void bind();
        void unbind();

        std::unique_ptr<CTextureCubeData>& data() { return m_data; }
        const std::unique_ptr<CTextureCubeData>& data() const { return m_data; }

        std::string name() const { return m_data->name; }
        uint32 width() const { return m_data->width; }
        uint32 height() const { return m_data->height; }
        uint32 channels() const { return m_data->channels; }
        eTextureFormat format() const { return m_data->format; }

        uint32 openglId() const { return m_openglId; }

    private :

        std::unique_ptr<CTextureCubeData> m_data;

        uint32 m_openglId;

    };

}
