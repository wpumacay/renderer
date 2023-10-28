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

    NO_COPY_NO_MOVE_NO_ASSIGN(TextureData)

 public:
    /// Creates a texture-data object from a given image
    explicit TextureData(const char* image_path);

    /// Creates a texture data object from given size and data
    explicit TextureData(int32_t width, int32_t height, int32_t channels,
                         const uint8_t* data);

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

}  // namespace renderer
