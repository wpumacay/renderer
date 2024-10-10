#pragma once

#include <string>
#include <unordered_map>

#include <renderer/engine/graphics/enums.hpp>
#include <renderer/engine/graphics/program_adapter_t.hpp>

namespace renderer {
namespace opengl {

/// Returns the respective OpenGL enum for the given shader type
auto ToOpenGLEnum(eShaderType type) -> uint32_t;

/// Compiles a shader given its source and shader type
auto CompileShader(const char* source, eShaderType type) -> uint32_t;

/// Adapter abstraction for the OpenGL backend of a Shader Program in GPU
class OpenGLProgramAdapter : public IProgramAdapter {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(OpenGLProgramAdapter)

    DEFINE_SMART_POINTERS(OpenGLProgramAdapter)
 public:
    OpenGLProgramAdapter() = default;

    ~OpenGLProgramAdapter() override;

    auto Build() -> void override;

    auto Bind() const -> void override;

    auto Unbind() const -> void override;

    auto SetInt(const char* uname, int32_t uvalue) -> void override;

    auto SetFloat(const char* uname, float uvalue) -> void override;

    auto SetVec2(const char* uname, const Vec2& uvalue) -> void override;

    auto SetVec3(const char* uname, const Vec3& uvalue) -> void override;

    auto SetVec4(const char* uname, const Vec4& uvalue) -> void override;

    auto SetMat4(const char* uname, const Mat4& uvalue) -> void override;

 private:
    /// Caches and returns the requested uniform location
    auto _GetUniformLocation(const char* uname) -> int32_t;

 private:
    // THe OpenGL ID associated to this program
    uint32_t m_OpenGLId = 0;

    /// Map used to keep uniforms' names and their locations
    std::unordered_map<std::string, int32_t> m_UniformLocationsCache;
};

}  // namespace opengl
}  // namespace renderer
