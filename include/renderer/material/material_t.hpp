#pragma once

#include <string>

#include <renderer/common.hpp>
#include <renderer/core/texture_t.hpp>

namespace renderer {

/// All available material types
enum class eMaterialType {
    /// Basic material type
    BASIC,
    /// Lambert material type
    LAMBERT,
    /// Blinn-Phong material type
    PHONG,
    /// Physically-based material type
    PBR,
};

/// Returns a string representation of the given material type Enum
auto ToString(const eMaterialType& mat_type) -> std::string;

/// Common interface for all avaialbe material types
class Material {
    // cppcheck-suppress unknownMacro
    DEFAULT_COPY_AND_MOVE_AND_ASSIGN(Material)

    DEFINE_SMART_POINTERS(Material)

 public:
    /// Whether the related object is visible or not in the scene
    bool visible = true;
    /// Whether the related object is transparent or not
    bool transparent = false;
    /// Whether the related object casts shadows or not
    bool castShadows = true;
    /// Whether the related object received shadows or not
    bool receiveShadows = true;
    /// The opacity of thie related object
    float opacity = 1.0F;
    /// The type of shading model used for rendering
    eMaterialType type = eMaterialType::BASIC;
    /// The ambient color component of this material
    Vec3 ambient = {0.8F, 0.3F, 0.5F};
    /// The diffuse color component of this material
    Vec3 diffuse = {0.8F, 0.3F, 0.5F};
    /// The specular color component of this material
    Vec3 specular = {0.8F, 0.3F, 0.5F};
    /// The power coefficient of the specular component
    float shininess = 32.0F;
    /// The albedo map used by this material
    ::renderer::Texture::ptr albedoMap = nullptr;
    /// The specular map used by this material
    ::renderer::Texture::ptr specularMap = nullptr;

 public:
    /// Releases all resources associated with this material
    virtual ~Material() = default;

    /// Returns a string representation of this material
    auto toString() const -> std::string;

 protected:
};

}  // namespace renderer
