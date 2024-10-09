#pragma once

#include <memory>
#include <string>

#include <renderer/common.hpp>
#include <renderer/engine/graphics/enums.hpp>

namespace renderer {

class Program;

/// Interface for program adapters, which link a shader object to its backend
class IProgramAdapter {
    NO_COPY_NO_MOVE_NO_ASSIGN(IProgramAdapter)

    DEFINE_SMART_POINTERS(IProgramAdapter)
 public:
    /// Creates an adapter to a specific backend
    IProgramAdapter() = default;

    /// Releases all resources in the specific backend
    virtual ~IProgramAdapter() = default;

    /// Sets a non-owning reference to the associated shader program object
    auto SetProgramHandle(std::shared_ptr<Program> program_handle) -> void {
        m_ProgramHandle = program_handle;
    }

    /// Links the program on the associated graphics API
    virtual auto Build() -> void = 0;

    /// Binds the current program for usage in a rendering pipeline
    virtual auto Bind() const -> void = 0;

    /// UNbinds the current program from the rendering pipeline
    virtual auto Unbind() const -> void = 0;

    /// Sets an int32 uniform given its name and desired value
    virtual auto SetInt(const char* uname, int32_t uvalue) -> void = 0;

    /// Sets a float32 uniform given its name and desired value
    virtual auto SetFloat(const char* uname, float uvalue) -> void = 0;

    /// Sets a vec-2 uniform given its name and desired value
    virtual auto SetVec2(const char* uname, const Vec2& uvalue) -> void = 0;

    /// Sets a vec-3 unbiform given its name and desired value
    virtual auto SetVec3(const char* uname, const Vec3& uvalue) -> void = 0;

    /// Sets a vec-4 unbiform given its name and desired value
    virtual auto SetVec4(const char* uname, const Vec4& uvalue) -> void = 0;

    /// Sets a mat-4 unbiform given its name and desired value
    virtual auto SetMat4(const char* uname, const Mat4& uvalue) -> void = 0;

    /// Checks if the associated program was build successfully
    RENDERER_NODISCARD auto IsValid() const -> bool { return m_IsValid; }

protected:
    /// Non-owning reference to a shader program object
    std::weak_ptr<Program> m_ProgramHandle;

    /// Flag that indicates whether or not the program build successfully
    bool m_IsValid = false;
};

}  // namespace renderer