#include <memory>

#include <spdlog/fmt/bundled/format.h>

#include <utils/logging.hpp>

#include <renderer/assets/shader_manager_t.hpp>

namespace renderer {

auto ShaderManager::LoadProgram(const std::string& name,
                                const std::string& vert_filepath,
                                const std::string& frag_filepath)
    -> Program::ptr {
    if (m_NumPrograms >= MAX_PROGRAMS) {
        LOG_WARN(
            "ShaderManager::LoadProgram >>> program cache is full; make it "
            "bigger?");
        return nullptr;
    }
    auto vert_src = utils::GetFileContents(vert_filepath.c_str());
    auto frag_src = utils::GetFileContents(frag_filepath.c_str());

    auto program = std::make_shared<Program>(name.c_str(), vert_src.c_str(),
                                             frag_src.c_str());
    auto prog_index = m_NumPrograms++;
    m_Programs.at(prog_index) = program;
    m_Name2Id[program->name()] = prog_index;

    return program;
}

auto ShaderManager::CacheProgram(Program::ptr program) -> void {
    if (program == nullptr) {
        LOG_WARN("ShaderManager::CacheProgram >>> can't cache nullptr :/");
        return;
    }

    auto prog_name = program->name();
    if (m_Name2Id.find(prog_name) != m_Name2Id.end()) {
        LOG_WARN(
            "ShaderManager::CacheProgram  >>> a program with the same name "
            "'{0}' already exists. Will keep older one",
            prog_name);
        return;
    }

    auto prog_index = m_NumPrograms++;
    m_Programs.at(prog_index) = std::move(program);
    m_Name2Id[prog_name] = prog_index;
}

auto ShaderManager::GetProgram(const std::string& name) -> Program::ptr {
    if (m_Name2Id.find(name) == m_Name2Id.end()) {
        LOG_CORE_ERROR(
            "ShaderManager::GetProgram >>> couldn't find program '{0}'", name);
        return nullptr;
    }

    auto prog_index = m_Name2Id[name];
    return m_Programs.at(prog_index);
}

auto ShaderManager::DeleteProgram(const std::string& name) -> void {
    if (m_Name2Id.find(name) == m_Name2Id.end()) {
        LOG_WARN(
            "ShaderManager::DeleteProgram >>> tried to delete non-existent "
            "program with id '{0}'",
            name);
        return;
    }

    auto prog_index = m_Name2Id[name];
    m_Programs.at(prog_index) = nullptr;
    // Rearrange the remaining items (shift left)
    for (uint32_t i = prog_index + 1; i < m_NumPrograms; ++i) {
        m_Programs.at(i - 1) = std::move(m_Programs.at(i));
    }
    m_NumPrograms--;
}

auto ShaderManager::GetProgramByIndex(uint32_t prog_index) -> Program::ptr {
    if (prog_index >= m_NumPrograms) {
        LOG_WARN(
            "ShaderManager::GetProgramByIndex >>> index '{0}' out of range "
            "[0-{1}]",
            prog_index, m_NumPrograms - 1);
        return nullptr;
    }

    return m_Programs.at(prog_index);
}

auto ShaderManager::ToString() const -> std::string {
    auto str_repr = fmt::format(
        "<ShaderManager\n"
        "  num_programs: {0}\n"
        "  programs: \n",
        m_NumPrograms);
    for (size_t i = 0; i < m_NumPrograms; ++i) {
        str_repr +=
            fmt::format("    name: {0}, opengl_id: {1}, ok: {2}\n",
                        m_Programs.at(i)->name(), m_Programs.at(i)->opengl_id(),
                        (m_Programs.at(i)->linked()) ? "true" : "false");
    }
    str_repr += ">\n";
    return str_repr;
}

}  // namespace renderer
