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

    auto LoadTexture(const std::string& tex_id, const std::string& filepath)
        -> Texture::ptr;

    auto CacheTexture(const std::string& tex_id, Texture::ptr) -> void;

    auto GetTexture(const std::string& tex_id) -> Texture::ptr;

    auto DeleteTexture(const std::string& tex_id) -> void;

    auto GetTextureByIndex(uint32_t tex_index) -> Texture::ptr;

    auto GetNumTextures() const -> uint32_t { return m_NumTextures; }

 private:
    /// Storage for our textures
    std::array<Texture::ptr, MAX_TEXTURES> m_Textures;
    /// Counter for the number of textures currently being managed
    uint32_t m_NumTextures = 0;
    /// Map for string-key to array-index
    std::unordered_map<std::string, uint32_t> m_Name2Id;
};

}  // namespace renderer
