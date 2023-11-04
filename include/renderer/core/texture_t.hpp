#pragma once

#include <string>
#include <cstdint>
#include <memory>

#include <renderer/common.hpp>
#include <renderer/core/texture_data_t.hpp>

/**
 * References:
 * [1]: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
 */

namespace renderer {

/// Available wrapping modes for a texture
enum class eTextureWrap {
    REPEAT,
    REPEAT_MIRROR,
    CLAMP_TO_EDGE,
    CLAMP_TO_BORDER,
};

/// Returns the string representation of the given texture wrapping mode
auto ToString(const eTextureWrap& tex_wrap) -> std::string;

/// Returns the associated OpenGL enum for the given texture wrapping mode
auto ToOpenGLEnum(const eTextureWrap& tex_wrap) -> int32_t;

/// Available texture filter options for a texture
enum class eTextureFilter {
    NEAREST,
    LINEAR,
    NEAREST_MIPMAP_NEAREST,
    LINEAR_MIPMAP_NEAREST,
    NEAREST_MIPMAP_LINEAR,
    LINEAR_MIPMAP_LINEAR,
};

/// Returns a string representation of the given texture filter option
auto ToString(const eTextureFilter& tex_filter) -> std::string;

/// Returns the associated OpenGL enum for this given texture filter options
auto ToOpenGLEnum(const eTextureFilter& tex_filter) -> int32_t;

/// Available internal formats types for a texture
enum class eTextureIntFormat {
    RED,
    RG,
    RGB,
    RGBA,
    DEPTH,
    DEPTH_STENCIL,
};

/// Returns the string representation of the given internal format type
auto ToString(const eTextureIntFormat& tex_iformat) -> std::string;

/// Returns the associated OpenGL enum of the given internal format type
auto ToOpenGLEnum(const eTextureIntFormat& tex_iformat) -> int32_t;

/// Texture object, representing an OpenGL texture
class Texture {
    // cppcheck-suppress unknownMacro
    DEFINE_SMART_POINTERS(Texture)

    NO_COPY_NO_MOVE_NO_ASSIGN(Texture)

 public:
    /// Creates a texture object from a given image
    explicit Texture(const char* image_path);

    /// Creates a texture object from given texture data
    explicit Texture(TextureData::ptr tex_data);

    /// Releases all resources allocated by this texture
    ~Texture();

    /// \brief Binds the current texture
    auto Bind() const -> void;

    /// \brief Unbinds the current texture
    auto Unbind() const -> void;

    /// \brief Returns a string representation for this texture
    auto ToString() const -> std::string;

    auto SetBorderColor(const Vec4& color) -> void;

    auto SetMinFilter(const eTextureFilter& tex_filter) -> void;

    auto SetMagFilter(const eTextureFilter& tex_filter) -> void;

    auto SetWrapModeU(const eTextureWrap& tex_wrap) -> void;

    auto SetWrapModeV(const eTextureWrap& tex_wrap) -> void;

    auto opengl_id() const -> uint32_t { return m_OpenGLId; }

    auto border_color() const -> Vec4 { return m_BorderColor; }

    auto internal_format() const -> eTextureIntFormat { return m_IntFormat; }

    auto min_filter() const -> eTextureFilter { return m_MinFilter; }

    auto mag_filter() const -> eTextureFilter { return m_MagFilter; }

    auto wrap_mode_u() const -> eTextureWrap { return m_WrapU; }

    auto wrap_mode_v() const -> eTextureWrap { return m_WrapV; }

    auto texture_data() -> TextureData::ptr { return m_TextureData; }

 private:
    /// Initializes the texture
    auto _InitializeTexture() -> void;

 private:
    /// Id of the OpenGL resource allocated on the GPU
    uint32_t m_OpenGLId = 0;
    /// Color used at the border (U|horizontal coordinate)
    Vec4 m_BorderColor;
    /// Internal format (number of color components of the texture). See [1]
    eTextureIntFormat m_IntFormat = eTextureIntFormat::RGB;
    /// Filter used for minification
    eTextureFilter m_MinFilter = eTextureFilter::NEAREST;
    /// Filter used for magnification
    eTextureFilter m_MagFilter = eTextureFilter::NEAREST;
    /// Wrapping mode (U|horizontal coordinate)
    eTextureWrap m_WrapU = eTextureWrap::REPEAT;
    /// Wrapping mode (V|vertical coordinate)
    eTextureWrap m_WrapV = eTextureWrap::REPEAT;
    /// Texture data (contains the image data)
    TextureData::ptr m_TextureData = nullptr;
};

}  // namespace renderer
