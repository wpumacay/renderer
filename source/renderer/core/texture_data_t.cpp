#include <cstring>

#include <glad/gl.h>

#include <spdlog/fmt/bundled/format.h>

#include <utils/logging.hpp>
#include <renderer/core/texture_data_t.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace renderer {

auto ToString(const eTextureFormat& format) -> std::string {
    switch (format) {
        case eTextureFormat::RGB:
            return "rgb";
        case eTextureFormat::RGBA:
            return "rgba";
        case eTextureFormat::BGRA:
            return "bgra";
        case eTextureFormat::DEPTH:
            return "depth";
        case eTextureFormat::STENCIL:
            return "stencil";
        default:
            return "undefined";
    }
}

auto ToOpenGLEnum(const eTextureFormat& format) -> uint32_t {
    switch (format) {
        case eTextureFormat::RGB:
            return GL_RGB;
        case eTextureFormat::RGBA:
            return GL_RGBA;
        case eTextureFormat::BGRA:
            return GL_BGRA;
        case eTextureFormat::DEPTH:
            return GL_DEPTH_COMPONENT;
        case eTextureFormat::STENCIL:
            return GL_STENCIL_INDEX;
        default:
            return GL_RGB;
    }
}

auto ToString(const eStorageType& dtype) -> std::string {
    switch (dtype) {
        case eStorageType::UINT_8:
            return "uint_8";
        case eStorageType::UINT_32:
            return "uint_32";
        case eStorageType::FLOAT_32:
            return "float_32";
        default:
            return "undefined";
    }
}

auto ToOpenGLEnum(const eStorageType& dtype) -> uint32_t {
    switch (dtype) {
        case eStorageType::UINT_8:
            return GL_UNSIGNED_BYTE;
        case eStorageType::UINT_32:
            return GL_UNSIGNED_INT;
        case eStorageType::FLOAT_32:
            return GL_FLOAT;
        default:
            return GL_UNSIGNED_BYTE;
    }
}

TextureData::TextureData(const char* image_path) : m_ImagePath(image_path) {
    if (m_ImagePath.find(".jpg") != std::string::npos ||
        m_ImagePath.find(".jpeg") != std::string::npos ||
        m_ImagePath.find(".JPG") != std::string::npos ||
        m_ImagePath.find(".JPEG") != std::string::npos) {
        m_Format = eTextureFormat::RGB;
        m_Storage = eStorageType::UINT_8;
    } else if (m_ImagePath.find(".png") != std::string::npos ||
               m_ImagePath.find(".PNG") != std::string::npos) {
        m_Format = eTextureFormat::RGBA;
        m_Storage = eStorageType::UINT_8;
    } else {
        m_Format = eTextureFormat::RGB;
        LOG_CORE_ERROR("TextureData >>> image-format not supported yet");
    }

    m_Data.reset(
        stbi_load(m_ImagePath.c_str(), &m_Width, &m_Height, &m_Channels, 0));
}

TextureData::TextureData(int32_t width, int32_t height, int32_t channels,
                         const uint8_t* data)
    : m_Width(width), m_Height(height), m_Channels(channels) {
    if (channels == 3) {
        m_Format = eTextureFormat::RGB;
    } else if (channels == 4) {
        m_Format = eTextureFormat::RGBA;
    }

    auto buffer_size =
        static_cast<size_t>(width * height * channels) * sizeof(uint8_t);
    m_Data = std::make_unique<uint8_t[]>(buffer_size);  // NOLINT
    memcpy(m_Data.get(), data, buffer_size);
}

auto TextureData::ToString() const -> std::string {
    return fmt::format(
        "<TextureData\n"
        "  width: {0}\n"
        "  height: {1}\n"
        "  channels: {2}\n"
        "  format: {3}\n"
        "  storage: {4}\n"
        "  image_path: {5}\n"
        ">\n",
        m_Width, m_Height, m_Channels, ::renderer::ToString(m_Format),
        ::renderer::ToString(m_Storage), m_ImagePath);
}

}  // namespace renderer
