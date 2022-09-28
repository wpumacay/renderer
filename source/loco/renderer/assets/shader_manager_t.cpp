#include <memory>

#include <loco/utils/common.hpp>
#include <loco/utils/logging.hpp>

#include <loco/renderer/assets/shader_manager_t.hpp>

namespace loco {
namespace renderer {

auto ShaderManager::LoadProgram(const std::string& name,
                                const std::string& vert_filepath,
                                const std::string& frag_filepath)
    -> Program::ptr {
    auto vert_src = utils::GetFileContents(vert_filepath.c_str());
    auto frag_src = utils::GetFileContents(frag_filepath.c_str());

    auto program = std::make_shared<Program>(name.c_str(), vert_src.c_str(),
                                             frag_src.c_str());

    m_Programs[program->name()] = program;
    return program;
}

auto ShaderManager::CacheProgram(Program::uptr program) -> void {
    auto program_name = program->name();
    m_Programs[program_name] = std::move(program);
}

auto ShaderManager::GetProgram(const std::string& name) -> Program::ptr {
    if (m_Programs.find(name) == m_Programs.end()) {
        LOG_CORE_ERROR(
            "ShaderManager::GetProgram >>> couldn't find program '{0}'", name);
        return nullptr;
    }
    return m_Programs[name];
}

}  // namespace renderer
}  // namespace loco
