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

auto TextureData::ToString() const -> std::string {
    std::string str_repr = "TextureData";
    str_repr += fmt::format("(format={0}, width={1}, height={2}, channels={3})",
                            renderer::ToString(m_Format), m_Width, m_Height,
                            m_Channels);
    return str_repr;
}

}  // namespace renderer
