#include <renderer/engine/object_t.hpp>
#include <renderer/engine/scene_t.hpp>

#include <spdlog/fmt/bundled/format.h>

#include <utility>
#include <utils/logging.hpp>

namespace renderer {

auto ToString(ObjectType type) -> std::string {
    switch (type) {
        case ObjectType::BASE:
            return "Base";
        case ObjectType::MESH:
            return "Mesh";
        default:
            return "Base";
    }
}

Object3D::Object3D(std::string name) : m_Name(std::move(name)) {}

Object3D::Object3D(std::string name, Pose init_pose)
    : m_Name(std::move(name)), m_WorldPose(init_pose) {}

Object3D::Object3D(std::string name, Vec3 init_pos, Quat init_quat)
    : m_Name(std::move(name)), m_WorldPose(Pose(init_pos, init_quat)) {}

auto Object3D::SetScene(std::weak_ptr<Scene> scene_handle) -> void {
    m_Scene = std::move(scene_handle);
}

auto Object3D::SetName(std::string new_name) -> void {
    if (!m_Scene.expired()) {
        LOG_CORE_WARN(
            "Object3D::SetName >>> Be careful that {0} already belongs to a "
            "scene, where it might be stored by its name as key",
            new_name);
    }
    m_Name = std::move(new_name);
}

auto Object3D::SetPose(Pose new_pose) -> void { m_WorldPose = new_pose; }

auto Object3D::SetPosition(Vec3 new_position) -> void {
    m_WorldPose.position = new_position;
}

auto Object3D::SetOrientation(Quat new_quat) -> void {
    m_WorldPose.orientation = new_quat;
}

auto Object3D::SetLocalPose(Pose pose) -> void { m_LocalPose = pose; }

auto Object3D::SetLocalPosition(Vec3 position) -> void {
    m_LocalPose.position = position;
}

auto Object3D::SetLocalOrientation(Quat orientation) -> void {
    m_LocalPose.orientation = orientation;
}

auto Object3D::ToString() const -> std::string {
    std::string str_repr = "Object3D<\n";
    str_repr += fmt::format("  name={0},\n", m_Name);
    str_repr +=
        fmt::format("  position={1},\n", m_WorldPose.position.toString());
    str_repr +=
        fmt::format("  orientation={2},\n", m_WorldPose.orientation.toString());
    if (auto parent = m_Parent.lock()) {
        str_repr += fmt::format("  parent={3},\n", parent->name());
    }
    if (!m_Children.empty()) {
        str_repr += "  children=";
        for (const auto& child : m_Children) {
            str_repr += child->name() + ",";
        }
        str_repr += "\n";
    }
    str_repr += ">\n";
    return str_repr;
}

}  // namespace renderer
