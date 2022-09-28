#pragma once

#include <string>
#include <unordered_map>

#include <loco/renderer/shader/program_t.hpp>

namespace loco {
namespace renderer {

/// Resource handler for shader programs
class ShaderManager {
    // cppcheck-suppress unknownMacro
    LOCO_NO_COPY_NO_MOVE_NO_ASSIGN(ShaderManager)

    LOCO_DEFINE_SMART_POINTERS(ShaderManager)

 public:
    ShaderManager() = default;

    /// Releases all resources allocated by this manager (i.e. all programs)
    ~ShaderManager() = default;

    /// Creates a shader program from the given vertex and fragment shader files
    auto LoadProgram(const std::string& name, const std::string& vert_filepath,
                     const std::string& frag_filepath) -> Program::ptr;

    /// Caches the given shader program by taking ownership of the resource
    auto CacheProgram(Program::uptr program) -> void;

    /// Returns a shader program with the given name (if not, returns  nullptr)
    auto GetProgram(const std::string& name) -> Program::ptr;

 private:
    /// Storage where to cache our loaded shader programs
    std::unordered_map<std::string, Program::ptr> m_Programs;
};

}  // namespace renderer
}  // namespace loco
