#pragma once

#include <string>
#include <cstdint>

#include <renderer/common.hpp>

namespace renderer {

/// Available format for the type of data stored in general textures
enum class eTextureFormat {
    RGB,
    RGBA,
    BGRA,
    DEPTH,
    STENCIL,
};

/// Returns the string representation of the given texture format
auto ToString(const eTextureFormat& format) -> std::string;

/// Returns the given format's associated OpenGL type enum
auto ToOpenGLEnum(const eTextureFormat& format) -> uint32_t;

/// Available storage options for a buffer of memory (how it's represented)
enum class eStorageType {
    UINT_8,
    UINT_32,
    FLOAT_32,
};

/// Returns the string representation of a given eStorageType
auto ToString(const eStorageType& dtype) -> std::string;

/// Returns the corresponding OpenGL enum for a given eStorageType
auto ToOpenGLEnum(const eStorageType& dtype) -> uint32_t;

/// Texture Data object (represents generally a texture's image data)
class TextureData {
    // cppcheck-suppress unknownMacro
    DEFINE_SMART_POINTERS(TextureData)

    NO_COPY_NO_MOVE_NO_ASSIGN(TextureData);

 public:
    /// Creates a texture-data object from a given image
    explicit TextureData(const char* image_path);

    /// Releases all allocated resources
    ~TextureData() = default;

    auto width() const -> int32_t { return m_Width; }

    auto height() const -> int32_t { return m_Height; }

    auto channels() const -> int32_t { return m_Channels; }

    auto image_path() const -> std::string { return m_ImagePath; }

    auto format() const -> eTextureFormat { return m_Format; }

    auto storage() const -> eStorageType { return m_Storage; }

    auto data() -> uint8_t* { return m_Data.get(); }

    auto data() const -> const uint8_t* { return m_Data.get(); }

    auto ToString() const -> std::string;

 private:
    /// Width of the texture image (if applicable)
    int32_t m_Width = 0;
    /// Height of the texture image (if applicable)
    int32_t m_Height = 0;
    /// Channels (depth) of the texture image (if applicable)
    int32_t m_Channels = 0;
    /// Data type used for storage of a pixel
    eStorageType m_Storage = eStorageType::UINT_8;
    /// Format of this texture data
    eTextureFormat m_Format = eTextureFormat::RGB;
    /// Path to the resource associated with this object (if applicable)
    std::string m_ImagePath{};
    /// Buffer for the memory used by this object's texture data
    std::unique_ptr<uint8_t[]> m_Data = nullptr;  // NOLINT
};

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

/// OpenGLTexture object, representing an OpenGL texture
class OpenGLTexture {
    // cppcheck-suppress unknownMacro
    DEFINE_SMART_POINTERS(OpenGLTexture)

    NO_COPY_NO_MOVE_NO_ASSIGN(OpenGLTexture);

 public:
    /// Creates a texture object from a given image
    explicit OpenGLTexture(const char* image_path);

    /// Creates a texture object from given texture data
    explicit OpenGLTexture(TextureData::uptr tex_data);

    /// Releases all resources allocated by this texture
    ~OpenGLTexture();

    // Binds the current texture
    auto Bind() const -> void;

    // Unbinds the current texture
    auto Unbind() const -> void;

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

    auto texture_data() -> TextureData::uptr& { return m_TextureData; }

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
    TextureData::uptr m_TextureData = nullptr;
};

}  // namespace renderer
