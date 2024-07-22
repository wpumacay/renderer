#include <renderer/engine/scene_t.hpp>
#include <renderer/engine/object_t.hpp>

#include <utils/logging.hpp>

namespace renderer {

auto Scene::AddObject(Object3D::ptr object) -> void {
    if (m_Name2Id.find(object->name()) != m_Name2Id.end()) {
        LOG_CORE_WARN(
            "Scene::AddObject >>> object {0} already exists in the scene. "
            "Won't add to avoid duplicates",
            object->name());
        return;
    }

    object->SetScene(shared_from_this());
    m_Objects.push_back(std::move(object));
    m_Name2Id[object->name()] = static_cast<ssize_t>(m_Objects.size() - 1);
}

auto Scene::ExistsObject(const std::string& name) -> bool {
    return m_Name2Id.find(name) != m_Name2Id.end();
}

auto Scene::RemoveObject(const std::string& name) -> void {
    if (!ExistsObject(name)) {
        LOG_CORE_WARN(
            "Scene::RemoveObject >>> object with name {0} doesn't "
            "exists in scene. Won't remove anything for the moment",
            name);
        return;
    }

    auto obj_idx = m_Name2Id.at(name);
    m_Objects.erase(m_Objects.begin() + obj_idx);
    m_Name2Id.erase(name);
    for (auto& [obj_name, idx] : m_Name2Id) {
        if (idx > obj_idx) {
            idx--;
        }
    }
}

auto Scene::GetObjectByName(const std::string& name) -> Object3D::ptr {
    if (!ExistsObject(name)) {
        return nullptr;
    }

    auto obj_idx = m_Name2Id.at(name);
    if (obj_idx < 0 || obj_idx >= static_cast<ssize_t>(m_Objects.size())) {
        LOG_CORE_WARN(
            "Scene::GetObjectByName >>> object {0} has index out of range {1} "
            "/ {2}",
            name, obj_idx, m_Objects.size());
        return nullptr;
    }

    return m_Objects[static_cast<size_t>(obj_idx)];
}

auto Scene::GetObjectByIndex(ssize_t index) -> Object3D::ptr {
    if (index < 0 || index >= static_cast<ssize_t>(m_Objects.size())) {
        LOG_CORE_WARN("Scene::GetObjectByIndex >>> index {0} out of range",
                      index);
    }

    return m_Objects[static_cast<size_t>(index)];
}

}  // namespace renderer
