#pragma once

#include <array>
#include <string>
#include <cstdint>
#include <unordered_map>

#include <renderer/common.hpp>
#include <renderer/shader/shader_t.hpp>

namespace renderer {

/// Max number of programmable stages
constexpr int MAX_SHADERS_IN_PROGRAM = 7;

/// Shader-Program class, used to handle a shader-program on the GPU
///
/// Defines the functionality of a Shader Program, which is used to interact
/// with a shader-program on GPU. This abstraction allows the user to update
/// uniforms and other properties of a GPU-linked shader program
class Program {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Program)

    DEFINE_SMART_POINTERS(Program)

 public:
    /// Creates a shader-program object with given name
    explicit Program(const char* name);

    /// Creates a shader-program object with given name and source code
    /// for vertex and fragment shaders
    /// \param[in] name Unique identifier of this shader program
    /// \param[in] vert_src Source code of the vertex-shader
    /// \param[in] frag_src Source code of the fragment-shader
    Program(const char* name, const char* vert_src, const char* frag_src);

    /// Adds the given shader to the list of shaders linked to this program
    auto AddShader(Shader::uptr shader) -> void;

    /// Links all the shaders associated with this program
    auto LinkProgram() -> void;

    /// Releases the resources allocated for this Shader Program on GPU
    ~Program();

    /// Binds the current program for usage in the rendering pipeline
    auto Bind() const -> void;

    /// Unbinds the current program from the rendering pipeline
    auto Unbind() const -> void;

    /// Sets an int32 uniform given its name and desired value
    /// \param[in] uname Name of the int32 uniform
    /// \param[in] uvalue Value of the int32 uniform
    auto SetInt(const char* uname, int32_t uvalue) -> void;

    /// Sets a float32 uniform given its name and desired value
    /// \param[in] uname Name of the float32 uniform
    /// \param[in] uvalue Value of the float32 uniform
    auto SetFloat(const char* uname, float uvalue) -> void;

    /// Sets a vec-2 uniform given its name and desired value
    /// \param[in] uname Name of the vec-2 uniform
    /// \param[in] uvalue Value of the vec-2 uniform
    auto SetVec2(const char* uname, const Vec2& uvalue) -> void;

    /// Sets a vec-3 unbiform given its name and desired value
    /// \param[in] uname Name of the vec-3 uniform
    /// \param[in] uvalue Value of the vec-3 uniform
    auto SetVec3(const char* uname, const Vec3& uvalue) -> void;

    /// Sets a vec-4 unbiform given its name and desired value
    /// \param[in] uname Name of the vec-4 uniform
    /// \param[in] uvalue Value of the vec-4 uniform
    auto SetVec4(const char* uname, const Vec4& uvalue) -> void;

    /// Sets a mat-4 unbiform given its name and desired value
    /// \param[in] uname Name of the mat-4 uniform
    /// \param[in] uvalue Value of the mat-4 uniform
    auto SetMat4(const char* uname, const Mat4& uvalue) -> void;

    /// Returns the uniform id for the given uniform name
    auto GetUniformLocation(const char* uname) -> int32_t;

    /// Returns the name of this program
    auto name() const -> std::string { return m_Name; }

    /// Returns the opengl-id of this shader on the GPU
    auto opengl_id() const -> uint32_t { return m_OpenGLId; }

    /// Returns whether or not if this shader has been compiled
    auto linked() const -> bool { return m_Linked; }

    /// Returns the number of shaders owned by this program
    auto num_shaders() const -> uint32_t { return m_NumShaders; }

 private:
    /// Unique name identifier of this shader
    std::string m_Name{};
    /// Resource ID of this program on the GPU
    uint32_t m_OpenGLId = 0;
    /// Whether or not this program is linked and ready to use
    bool m_Linked = false;
    /// Container for our shader objects
    std::array<Shader::uptr, MAX_SHADERS_IN_PROGRAM> m_Shaders;
    /// Counter of the current number of shaders associated to this program
    uint32_t m_NumShaders = 0;
    /// Map used to keep uniforms' names and their locations
    std::unordered_map<std::string, int32_t> m_UniformLocationsCache;
};

}  // namespace renderer
