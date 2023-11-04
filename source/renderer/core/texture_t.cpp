#include <glad/gl.h>

#include <spdlog/fmt/bundled/format.h>

#include <utils/logging.hpp>
#include <renderer/core/texture_t.hpp>

// References:
// [1] StackOverflow's Questions
// 23150123/loading-png-with-stb-image-for-opengl-texture-gives-wrong-colors

namespace renderer {

auto ToString(const eTextureWrap& tex_wrap) -> std::string {
    switch (tex_wrap) {
        case eTextureWrap::REPEAT:
            return "repeat";
        case eTextureWrap::REPEAT_MIRROR:
            return "repeat_mirror";
        case eTextureWrap::CLAMP_TO_EDGE:
            return "clamp_to_edge";
        case eTextureWrap::CLAMP_TO_BORDER:
            return "clamp_to_border";
        default:
            return "undefined";
    }
}

auto ToOpenGLEnum(const eTextureWrap& tex_wrap) -> int32_t {
    switch (tex_wrap) {
        case eTextureWrap::REPEAT:
            return GL_REPEAT;
        case eTextureWrap::REPEAT_MIRROR:
            return GL_MIRRORED_REPEAT;
        case eTextureWrap::CLAMP_TO_EDGE:
            return GL_CLAMP_TO_EDGE;
        case eTextureWrap::CLAMP_TO_BORDER:
            return GL_CLAMP_TO_BORDER;
        default:
            return GL_REPEAT;
    }
}

auto ToString(const eTextureFilter& tex_filter) -> std::string {
    switch (tex_filter) {
        case eTextureFilter::NEAREST:
            return "nearest";
        case eTextureFilter::LINEAR:
            return "linear";
        case eTextureFilter::NEAREST_MIPMAP_NEAREST:
            return "nearest_mipmap_nearest";
        case eTextureFilter::LINEAR_MIPMAP_NEAREST:
            return "linear_mipmap_nearest";
        case eTextureFilter::NEAREST_MIPMAP_LINEAR:
            return "nearest_mipmap_linear";
        case eTextureFilter::LINEAR_MIPMAP_LINEAR:
            return "linear_mipmap_linear";
        default:
            return "undefined";
    }
}

auto ToOpenGLEnum(const eTextureFilter& tex_filter) -> int32_t {
    switch (tex_filter) {
        case eTextureFilter::NEAREST:
            return GL_NEAREST;
        case eTextureFilter::LINEAR:
            return GL_LINEAR;
        case eTextureFilter::NEAREST_MIPMAP_NEAREST:
            return GL_NEAREST_MIPMAP_NEAREST;
        case eTextureFilter::LINEAR_MIPMAP_NEAREST:
            return GL_LINEAR_MIPMAP_NEAREST;
        case eTextureFilter::NEAREST_MIPMAP_LINEAR:
            return GL_NEAREST_MIPMAP_LINEAR;
        case eTextureFilter::LINEAR_MIPMAP_LINEAR:
            return GL_LINEAR_MIPMAP_LINEAR;
        default:
            return GL_NEAREST;
    }
}

auto ToString(const eTextureIntFormat& tex_iformat) -> std::string {
    switch (tex_iformat) {
        case eTextureIntFormat::RED:
            return "i_r";
        case eTextureIntFormat::RG:
            return "i_rg";
        case eTextureIntFormat::RGB:
            return "i_rgb";
        case eTextureIntFormat::RGBA:
            return "i_rgba";
        case eTextureIntFormat::DEPTH:
            return "i_depth";
        case eTextureIntFormat::DEPTH_STENCIL:
            return "i_stencil";
        default:
            return "undefined";
    }
}

auto ToOpenGLEnum(const eTextureIntFormat& tex_iformat) -> int32_t {
    switch (tex_iformat) {
        case eTextureIntFormat::RED:
            return GL_RED;
        case eTextureIntFormat::RG:
            return GL_RG;
        case eTextureIntFormat::RGB:
            return GL_RGB;
        case eTextureIntFormat::RGBA:
            return GL_RGBA;
        case eTextureIntFormat::DEPTH:
            return GL_DEPTH_COMPONENT;
        case eTextureIntFormat::DEPTH_STENCIL:
            return GL_DEPTH_STENCIL;
        default:
            return GL_RGB;
    }
}

Texture::Texture(const char* image_path) {
    m_TextureData = std::make_shared<TextureData>(image_path);

    if (m_TextureData->data() == nullptr) {
        LOG_CORE_ERROR(
            "Texture >>> There was an issue loading the texture data for path "
            "{0}",
            image_path);
        return;
    }

    _InitializeTexture();
}

Texture::Texture(TextureData::ptr tex_data) {
    if (tex_data->data() == nullptr) {
        LOG_CORE_ERROR(
            "Texture >>> There was an issue with the given texture.");
        return;
    }

    m_TextureData = std::move(tex_data);

    _InitializeTexture();
}

auto Texture::_InitializeTexture() -> void {
    glGenTextures(1, &m_OpenGLId);
    glBindTexture(GL_TEXTURE_2D, m_OpenGLId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToOpenGLEnum(m_WrapU));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToOpenGLEnum(m_WrapV));
    if (m_WrapU == eTextureWrap::CLAMP_TO_BORDER ||
        m_WrapV == eTextureWrap::CLAMP_TO_BORDER) {
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,
                         m_BorderColor.data());
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    ToOpenGLEnum(m_MinFilter));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    ToOpenGLEnum(m_MagFilter));

    if (m_TextureData->data() != nullptr) {
        // --------------------------------
        // If we're loading from an image, the use the same internal format
        if (m_TextureData->format() == eTextureFormat::RGBA) {
            m_IntFormat = eTextureIntFormat::RGBA;
        } else if (m_TextureData->format() == eTextureFormat::RGB) {
            m_IntFormat = eTextureIntFormat::RGB;
        }  // otherwise, use the intformat selected by the user
        // --------------------------------

        // FIX(wilbert): no rows-alignment as expected from OpenGL (fixes issue
        // with images loaded using stbi_load). See reference [1]
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(
            GL_TEXTURE_2D, 0, ToOpenGLEnum(m_IntFormat), m_TextureData->width(),
            m_TextureData->height(), 0, ToOpenGLEnum(m_TextureData->format()),
            ToOpenGLEnum(m_TextureData->storage()), m_TextureData->data());
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() {
    m_TextureData = nullptr;
    if (m_OpenGLId != 0) {
        glDeleteTextures(1, &m_OpenGLId);
        m_OpenGLId = 0;
    }
}

auto Texture::Bind() const -> void {
    // This bind method assumes we're only dealing with a single texture unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_OpenGLId);
}

// NOLINTNEXTLINE
auto Texture::Unbind() const -> void {
    // This unbind method assumes we're only dealing with a single texture unit
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Texture::ToString() const -> std::string {
    return fmt::format(
        "<Texture\n"
        "  width: {0}\n"
        "  height: {1}\n"
        "  channels: {2}\n"
        "  borderColor: {3}\n"
        "  minFilter: {4}\n"
        "  magFilter: {5}\n"
        "  wrapModeU: {6}\n"
        "  wrapModeV: {7}\n"
        "  openGLid: {8}\n"
        ">\n",
        (m_TextureData != nullptr ? m_TextureData->width() : 0),
        (m_TextureData != nullptr ? m_TextureData->height() : 0),
        (m_TextureData != nullptr ? m_TextureData->channels() : 0),
        m_BorderColor.toString(), ::renderer::ToString(m_MinFilter),
        ::renderer::ToString(m_MagFilter), ::renderer::ToString(m_WrapU),
        ::renderer::ToString(m_WrapV), m_OpenGLId);
}

auto Texture::SetBorderColor(const Vec4& color) -> void {
    m_BorderColor = color;
    glBindTexture(GL_TEXTURE_2D, m_OpenGLId);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR,
                     m_BorderColor.data());
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Texture::SetMinFilter(const eTextureFilter& tex_filter) -> void {
    m_MinFilter = tex_filter;
    glBindTexture(GL_TEXTURE_2D, m_OpenGLId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    ToOpenGLEnum(m_MinFilter));
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Texture::SetMagFilter(const eTextureFilter& tex_filter) -> void {
    m_MagFilter = tex_filter;
    glBindTexture(GL_TEXTURE_2D, m_OpenGLId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    ToOpenGLEnum(m_MagFilter));
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Texture::SetWrapModeU(const eTextureWrap& tex_wrap) -> void {
    m_WrapU = tex_wrap;
    glBindTexture(GL_TEXTURE_2D, m_OpenGLId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToOpenGLEnum(m_WrapU));
    glBindTexture(GL_TEXTURE_2D, 0);
}

auto Texture::SetWrapModeV(const eTextureWrap& tex_wrap) -> void {
    m_WrapV = tex_wrap;
    glBindTexture(GL_TEXTURE_2D, m_OpenGLId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToOpenGLEnum(m_WrapV));
    glBindTexture(GL_TEXTURE_2D, 0);
}

}  // namespace renderer
