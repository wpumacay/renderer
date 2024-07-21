#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <renderer/common.hpp>

namespace renderer {

class Object3D;

class Scene : public std::enable_shared_from_this<Scene> {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Scene)

    DEFINE_SMART_POINTERS(Scene)

 public:
    /// Creates an empty scene
    Scene() = default;

    /// Deallocates any owned resources by this scene
    ~Scene() = default;

    /// Add object to this scene
    /// \param[in] object The object to be added to this scene
    auto AddObject(std::shared_ptr<Object3D> object) -> void;

    /// Returns a vector with all objects owned by this scene
    auto GetObjects() -> std::vector<std::shared_ptr<Object3D>> {
        return m_Objects;
    }

 protected:
    /// Storage for shared ownership of Object3D objects
    std::vector<std::shared_ptr<Object3D>> m_Objects;

    /// Map used for storing object names and link to index in storage
    std::unordered_map<std::string, size_t> m_Name2Id;
};

}  // namespace renderer
