#pragma once

#include <memory>
#include <string>
#include <vector>

#include <renderer/common.hpp>

namespace renderer {

class Object3D {
    DEFAULT_COPY_AND_MOVE_AND_ASSIGN(Object3D)

    DEFINE_SMART_POINTERS(Object3D)

 public:
    /// Creates a default Object3D
    Object3D() = default;

    /// Creates an Object3D with the given name
    /// \param[in] name The name of this object
    explicit Object3D(std::string name);

    /// Creates an Object3D with the given name and with given pose in world
    /// space
    /// \param[in] name The name of this object
    /// \param[in] init_pose The pose of this object in world space
    Object3D(std::string name, Pose init_pose);

    /// Creates an Object3D with the given name, position and orientation in
    /// world space
    /// \param[in] name The name of this object
    /// \param[in] init_pos The position of this object in world space
    /// \param[in] init_quat The orientation of this object in world space
    Object3D(std::string name, Vec3 init_pos, Quat init_quat);

    /// Deallocates the resources used by this object
    virtual ~Object3D() = default;

    /// Sets the name of this object
    /// \param[in]  new_name The new name to use for this object
    auto SetName(std::string new_name) -> void;

    /// Sets whether or not this object belongs to a scene

    auto SetInScene(bool is_in_scene) -> void;

    /// Sets the pose of this object in world space
    /// \param[in] new_pose The new pose of this object in world space
    virtual auto SetPose(Pose new_pose) -> void;

    /// Sets the position of this object in world space
    /// \param[in] new_position The new position of this object in world space
    virtual auto SetPosition(Vec3 new_position) -> void;

    /// Sets the orientation of this object in world space
    /// \param[in] new_quat The new orientation of this object in world space
    virtual auto SetOrientation(Quat new_quat) -> void;

    /// Sets the local pose of this object with respect to its parent
    /// \param[in] pose The local pose of this object respect to its parent
    virtual auto SetLocalPose(Pose pose) -> void;

    /// Sets the local position of this object with respect to its parent
    /// \param[in] position The local position of this object to its parent
    virtual auto SetLocalPosition(Vec3 position) -> void;

    /// Sets the local orientation of this object with respect to its parent
    /// \param[in] orientation The local orientation of this object
    virtual auto SetLocalOrientation(Quat orientation) -> void;

    /// Returns the pose in world space of this object
    [[nodiscard]] auto pose() const -> Pose;

    /// Returns the position in world space of this object
    [[nodiscard]] auto position() const -> Vec3;

    /// Returns the orientation in world space of this object
    [[nodiscard]] auto orientation() const -> Quat;

    /// Returns the local pose of this object with respect to its parent
    [[nodiscard]] auto local_pose() const -> Pose { return m_LocalPose; }

    /// Returns the local position of this object with respect to its parent
    [[nodiscard]] auto local_position() const -> Vec3 {
        return m_LocalPose.position;
    }

    /// Returns the local orientation of this object with respect to its parent
    [[nodiscard]] auto local_orientation() const -> Quat {
        return m_LocalPose.orientation;
    }

    /// Returns the name of this object
    [[nodiscard]] auto name() -> std::string { return m_Name; }

    /// Returns a string representation of this object
    [[nodiscard]] auto ToString() const -> std::string;

 protected:
    /// The name of this object
    std::string m_Name;

    /// Flag indicating whether or not this object belongs to a scene
    bool m_InScene = false;

    /// The 3d pose of this object respect to world space
    Pose m_WorldPose;

    /// The 3d pose of this object respect to its parent (if applicable)
    Pose m_LocalPose;

    /// A weak reference to the parent of this object
    std::weak_ptr<Object3D> m_Parent;

    /// A container to hold shared ownership of any children
    std::vector<Object3D::ptr> m_Children;
};

}  // namespace renderer
