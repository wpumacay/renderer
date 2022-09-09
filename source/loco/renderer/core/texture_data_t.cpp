
#include <glad/gl.h>

#include <spdlog/fmt/bundled/format.h>

#include <loco/renderer/core/texture_data_t.hpp>
#include "spdlog/fmt/bundled/core.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace loco {
namespace renderer {

auto ToString(const eTextureFormat& format) -> std::string {
    switch (format) {
        case eTextureFormat::RGB:
            return "rgb";
        case eTextureFormat::RGBA:
            return "rgba";
        case eTextureFormat::DEPTH:
            return "depth";
        case eTextureFormat::STENCIL:
            return "stencil";
        case eTextureFormat::DEPTH24_STENCIL8:
            return "depth24_stencil8";
    }
}

auto ToOpenGLEnum(const eTextureFormat& format) -> uint32_t {
    switch (format) {
        case eTextureFormat::RGB:
            return GL_RGB;
        case eTextureFormat::RGBA:
            return GL_RGBA;
        case eTextureFormat::DEPTH:
            return GL_DEPTH_COMPONENT;
        case eTextureFormat::STENCIL:
            return GL_STENCIL_INDEX;
        case eTextureFormat::DEPTH24_STENCIL8:
            return GL_DEPTH24_STENCIL8;
    }
}

TextureData::TextureData(const char* image_path) : m_ImagePath(image_path) {
    if (m_ImagePath.find(".jpg") != std::string::npos ||
        m_ImagePath.find(".jpeg") != std::string::npos ||
        m_ImagePath.find(".JPG") != std::string::npos ||
        m_ImagePath.find(".JPEG") != std::string::npos) {
        m_Format = eTextureFormat::RGB;
    } else if (m_ImagePath.find(".png") != std::string::npos ||
               m_ImagePath.find(".PNG") != std::string::npos) {
        m_Format = eTextureFormat::RGBA;
    }

    m_Data.reset(
        stbi_load(m_ImagePath.c_str(), &m_Width, &m_Height, &m_Channels, 0));
}

auto TextureData::ToString() const -> std::string {
    std::string str_repr = "TextureData";
    str_repr += fmt::format("(format={0}, width={1}, height={2}, channels={3})",
                            renderer::ToString(m_Format), m_Width, m_Height,
                            m_Channels);
    return str_repr;
}

}  // namespace renderer
}  // namespace loco
