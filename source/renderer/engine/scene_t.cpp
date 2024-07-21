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
    m_Name2Id[object->name()] = m_Objects.size() - 1;
}

}  // namespace renderer
