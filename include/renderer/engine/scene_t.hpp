#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <renderer/common.hpp>

namespace renderer {

class Object3D;

/// Scene container for engine objects of various types
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

    /// Returns whether or not an object with given name exists in the scene
    /// \param[in] name The name of the object to be searched
    auto ExistsObject(const std::string& name) -> bool;

    /// Removes the object with given name
    /// \param[in] name The name of the object to be deleted
    auto RemoveObject(const std::string& name) -> void;

    /// Returns the object requested by name
    /// \param[in] name The name of the object which we're trying to find
    [[nodiscard]] auto GetObjectByName(const std::string& name)
        -> std::shared_ptr<Object3D>;

    /// Returns the object requested by index
    /// \param[in] idx The index of the object in the underlying container
    [[nodiscard]] auto GetObjectByIndex(ssize_t index)
        -> std::shared_ptr<Object3D>;

    /// Returns a vector with all objects owned by this scene
    [[nodiscard]] auto GetObjects() -> std::vector<std::shared_ptr<Object3D>> {
        return m_Objects;
    }

 protected:
    /// Storage for shared ownership of Object3D objects
    std::vector<std::shared_ptr<Object3D>> m_Objects;

    /// Map used for storing object names and link to index in storage
    std::unordered_map<std::string, ssize_t> m_Name2Id;
};

}  // namespace renderer
