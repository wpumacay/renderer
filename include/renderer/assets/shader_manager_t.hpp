#pragma once

#include <array>
#include <string>
#include <unordered_map>

#include <renderer/shader/program_t.hpp>

namespace renderer {

constexpr uint32_t MAX_PROGRAMS = 128;

/// Resource handler for shader programs
class ShaderManager {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(ShaderManager)

    DEFINE_SMART_POINTERS(ShaderManager)

 public:
    ShaderManager() = default;

    /// Releases all resources allocated by this manager (i.e. all programs)
    ~ShaderManager() = default;

    /// Creates a shader program from the given vertex and fragment shader files
    auto LoadProgram(const std::string& name, const std::string& vert_filepath,
                     const std::string& frag_filepath) -> Program::ptr;

    /// Caches the given shader program by taking ownership of the resource
    auto CacheProgram(Program::ptr program) -> void;

    /// Returns a shader program with the given name (if not, returns  nullptr)
    auto GetProgram(const std::string& name) -> Program::ptr;

    /// Deletes a shader program given its id
    auto DeleteProgram(const std::string& name) -> void;

    /// Returns a shader program given its index on the container
    auto GetProgramByIndex(uint32_t prog_index) -> Program::ptr;

    /// Returns the current number of shader programs being managed
    auto GetNumPrograms() const -> uint32_t { return m_NumPrograms; }

    /// \brief Returns the string representation of thie shader manager
    auto ToString() const -> std::string;

 private:
    /// Storage for our shader programs
    std::array<Program::ptr, MAX_PROGRAMS> m_Programs;
    /// Counter for the current number of programs being stored
    uint32_t m_NumPrograms = 0;
    /// Map for string-key to array-index
    std::unordered_map<std::string, uint32_t> m_Name2Id;
};

}  // namespace renderer
