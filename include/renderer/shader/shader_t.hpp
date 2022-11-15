#pragma once

#include <string>
#include <cstdint>

#include <renderer/common.hpp>
#include <utils/common.hpp>

namespace renderer {

/// Available shader types
enum class eShaderType {
    VERTEX,                   //< Associated to a vertex shasder
    FRAGMENT,                 //< Associated to a fragment shader
    GEOMETRY,                 //< Associated to a geometry shader
    COMPUTE,                  //< Associated to a compute shader
    TESSELLATION_CONTROL,     //< Associated to a tessellation-control shader
    TESSELLATION_EVALUATION,  //< Associated to a tessellation-evaluation shader
};

/// Returns the string representation of the given shader type
auto ToString(const eShaderType& type) -> std::string;

/// Returns the OpenGL enum corresponding to the given shader type
auto ToOpenGLEnum(const eShaderType& type) -> uint32_t;

/// Shader abstraction class, used to handle a shader on GPU
class Shader {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(Shader)

    DEFINE_SMART_POINTERS(Shader)

 public:
    /// Creates a shader from a set of given parameters
    /// \param[in] name Unique name identifier of this shader
    /// \param[in] type Type of the shader to be created on GPU
    /// \param[in] source Source code of the shader to be created on GPU
    Shader(const char* name, const eShaderType& type, const char* source);

    /// Releases the resources allocated by this shader on the GPU
    ~Shader();

    auto Release() -> void;

    /// Returns the name of this shader
    auto name() const -> std::string { return m_Name; }

    /// Returns the type of this shader
    auto type() const -> eShaderType { return m_Type; }

    /// Returns the source code of this shader
    auto source() const -> std::string { return m_Source; }

    /// Returns the opengl-id of this shader on the GPU
    auto opengl_id() const -> uint32_t { return m_OpenGLId; }

    /// Returns whether or not if this shader has been compiled
    auto compiled() const -> bool { return m_Compiled; }

 private:
    /// Compiles this shader and allocates a resource on the GPU
    auto _CompileShader(const char* source) -> void;

 private:
    /// Unique name of the shader
    std::string m_Name{};
    /// Type of this shader
    eShaderType m_Type = eShaderType::VERTEX;
    /// Source code attached to this shader
    std::string m_Source{};
    /// Resource ID of this shader on the GPU
    uint32_t m_OpenGLId = 0;
    /// Whether or not this shader is compiled and ready to use
    bool m_Compiled = false;
};

}  // namespace renderer
