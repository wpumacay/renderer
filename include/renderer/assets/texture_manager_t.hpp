#pragma once

#include <array>
#include <string>
#include <unordered_map>

#include <renderer/core/texture_t.hpp>

namespace renderer {

constexpr uint32_t MAX_TEXTURES = 128;

/// Resource handler for textures
class TextureManager {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(TextureManager)

    DEFINE_SMART_POINTERS(TextureManager)

 public:
    TextureManager() = default;

    ~TextureManager() = default;

    /// \brief Loads a texture from a file path, stores it and returns it
    auto LoadTexture(const std::string& tex_id, const std::string& filepath)
        -> Texture::ptr;

    /// \brief Caches the given texture with given id for later use
    auto CacheTexture(const std::string& tex_id, Texture::ptr) -> void;

    /// \brief Returns a cached texture with the given id
    auto GetTexture(const std::string& tex_id) -> Texture::ptr;

    /// \brief Deletes a texture with the given id (if exists)
    auto DeleteTexture(const std::string& tex_id) -> void;

    /// \brief Returns a cached texture given its index
    auto GetTextureByIndex(uint32_t tex_index) -> Texture::ptr;

    /// \brief Returns the number of textures cached by the manager
    auto GetNumTextures() const -> uint32_t { return m_NumTextures; }

    /// \brief Returns the string representation of the texture manager
    auto ToString() const -> std::string;

 private:
    /// Storage for our textures
    std::array<Texture::ptr, MAX_TEXTURES> m_Textures;

    /// Counter for the number of textures currently being managed
    uint32_t m_NumTextures = 0;

    /// Map for string-key to array-index
    std::unordered_map<std::string, uint32_t> m_Name2Id;
};

}  // namespace renderer
