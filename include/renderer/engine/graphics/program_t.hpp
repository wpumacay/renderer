#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <string>

#include <renderer/common.hpp>
#include <renderer/engine/graphics/enums.hpp>
#include <renderer/engine/graphics/program_adapter_t.hpp>

namespace renderer {

/// Shader-Program class, used to handle a shader-program on the GPU
class RENDERER_API Program : public std::enable_shared_from_this<Program> {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Program)

    DEFINE_SMART_POINTERS(Program)

 public:
    /// Creates a shader program object with default settings
    explicit Program(eGraphicsAPI api);

    /// Creates a shader-program object with given name and source code
    /// \param[in] vert_src Source code of the vertex-shader
    /// \param[in] frag_src Source code of the fragment-shader
    /// \param[in] api The graphics API to be used for this shader program
    explicit Program(const char* vert_src, const char* frag_src,
                     eGraphicsAPI api);

    /// Releases the resources allocated for this Shader Program on GPU
    ~Program() = default;

    /// Creates a shader-program object from a set of given parameters
    /// \param[in] vert_src Source code of the vertex shader
    /// \param[in] frag_src Source code of the fragment shader
    /// \param[in] api The graphics API to be used for this shader program
    static auto CreateProgram(const char* vert_src, const char* frag_src,
                              eGraphicsAPI api) -> std::shared_ptr<Program>;

    /// Initialize the program and backend related resources
    auto Initialize() -> void;

    /// Links all the shaders associated with this program
    auto Build() -> void;

    /// Binds the current program for usage in the rendering pipeline
    auto Bind() const -> void;

    /// Unbinds the current program from the rendering pipeline
    auto Unbind() const -> void;

    /// Sets an int32 uniform given its name and desired value
    auto SetInt(const char* uname, int32_t uvalue) -> void;

    /// Sets a float32 uniform given its name and desired value
    auto SetFloat(const char* uname, float uvalue) -> void;

    /// Sets a vec-2 uniform given its name and desired value
    auto SetVec2(const char* uname, const Vec2& uvalue) -> void;

    /// Sets a vec-3 unbiform given its name and desired value
    auto SetVec3(const char* uname, const Vec3& uvalue) -> void;

    /// Sets a vec-4 unbiform given its name and desired value
    auto SetVec4(const char* uname, const Vec4& uvalue) -> void;

    /// Sets a mat-4 unbiform given its name and desired value
    auto SetMat4(const char* uname, const Mat4& uvalue) -> void;

   /// Returns whether or not this shader is valid
    RENDERER_NODISCARD auto IsValid() const -> bool;

    /// Returns the code used for the vertex shader stage
    RENDERER_NODISCARD auto vertex_source() const -> std::string {
      return m_VertSource;
    }

    /// Returns the code used for the fragment shader stage
    RENDERER_NODISCARD auto fragment_source() const -> std::string {
      return m_FragSource;
    }

 private:
    /// Creates the internal adapter to link to the specific Graphics API
    auto _InitializeBackend() -> void;

 private:
    /// The graphics API to be used by this shader-program
    eGraphicsAPI m_API;

    /// Source code for the vertex shader stage
    std::string m_VertSource{};

    /// Source code for the fragment shader stage
    std::string m_FragSource{};

    /// Owning reference to a program adapter for a specific backend
    std::unique_ptr<IProgramAdapter> m_BackendAdapter = nullptr;
};

}  // namespace renderer
