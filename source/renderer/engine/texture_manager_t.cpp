#include <memory>

#include <spdlog/fmt/bundled/format.h>

#include <utils/logging.hpp>

#include <renderer/assets/texture_manager_t.hpp>

namespace renderer {

auto TextureManager::LoadTexture(const std::string& tex_id,
                                 const std::string& filepath) -> Texture::ptr {
    if (m_NumTextures >= MAX_TEXTURES) {
        LOG_WARN(
            "TextureManager::LoadTexture >>> texture cache full; make it "
            "bigger?");
        return nullptr;
    }

    auto texture = std::make_shared<Texture>(filepath.c_str());
    auto tex_index = m_NumTextures++;
    m_Textures.at(tex_index) = texture;
    m_Name2Id[tex_id] = tex_index;

    return texture;
}

auto TextureManager::CacheTexture(const std::string& tex_id,
                                  Texture::ptr texture) -> void {
    if (texture == nullptr) {
        LOG_WARN(
            "TextureManager::CacheTexture >>> can't cache nullptr :/ (while "
            "caching texture-id '{0}')",
            tex_id);
        return;
    }

    if (m_Name2Id.find(tex_id) != m_Name2Id.end()) {
        LOG_WARN(
            "TextureManager::CacheTexture >>> a texture with the same name "
            "'{0}' already exists. Won't duplicate :)",
            tex_id);
        return;
    }

    auto tex_index = m_NumTextures++;
    m_Textures.at(tex_index) = std::move(texture);
    m_Name2Id[tex_id] = tex_index;
}

auto TextureManager::GetTexture(const std::string& tex_id) -> Texture::ptr {
    if (m_Name2Id.find(tex_id) == m_Name2Id.end()) {
        LOG_WARN(
            "TextureManager::GetTexture >>> sorry, we couldn't find a texture "
            "with id '{0}'",
            tex_id);
        return nullptr;
    }

    auto tex_index = m_Name2Id[tex_id];
    return m_Textures.at(tex_index);
}

auto TextureManager::DeleteTexture(const std::string& tex_id) -> void {
    if (m_Name2Id.find(tex_id) == m_Name2Id.end()) {
        LOG_WARN(
            "TextureManager::DeleteTexture >>> tried to delete non-existent "
            "texture with id '{0}'",
            tex_id);
        return;
    }

    auto tex_index = m_Name2Id[tex_id];
    m_Textures.at(tex_index) = nullptr;
    // Rearrange the remaining items (shift left)
    for (uint32_t i = tex_index + 1; i < m_NumTextures; ++i) {
        m_Textures.at(i - 1) = std::move(m_Textures.at(i));
    }
    m_NumTextures--;
}

auto TextureManager::GetTextureByIndex(uint32_t tex_index) -> Texture::ptr {
    if (tex_index >= m_NumTextures) {
        LOG_WARN(
            "TextureManager::GetTextureByIndex >>> index '{0}' out of range "
            "[0-{1}]",
            tex_index, m_NumTextures - 1);
        return nullptr;
    }

    return m_Textures.at(tex_index);
}

auto TextureManager::ToString() const -> std::string {
    auto str_repr = fmt::format(
        "<TextureManager\n"
        "  num_textures: {0}\n"
        "  textures: \n",
        m_NumTextures);
    for (const auto& entry : m_Name2Id) {
        str_repr += fmt::format(
            "    name: {0}, width: {1}, height: {2}, channels: {3}, GLid: "
            "{4}\n",
            entry.first, m_Textures.at(entry.second)->texture_data()->width(),
            m_Textures.at(entry.second)->texture_data()->height(),
            m_Textures.at(entry.second)->texture_data()->channels(),
            m_Textures.at(entry.second)->opengl_id());
    }

    str_repr += ">\n";
    return str_repr;
}

}  // namespace renderer
