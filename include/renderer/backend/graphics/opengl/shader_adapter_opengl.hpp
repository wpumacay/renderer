#pragma once

#include <renderer/engine/graphics/enums.hpp>
#include <renderer/engine/graphics/shader_adapter_t.hpp>

namespace renderer {
namespace opengl {

/// Returns the respective OpenGL enum for the given shader type
auto ToOpenGLEnum(eShaderType type) -> uint32_t;

/// Implementation of a shader adapter for the OpenGL backend
class OpenGLShaderAdapter : public IShaderAdapter {
    // cppcheck-suppress unknownMacro
    NO_COPY_NO_MOVE_NO_ASSIGN(OpenGLShaderAdapter)

    DEFINE_SMART_POINTERS(OpenGLShaderAdapter)

   public:
    OpenGLShaderAdapter() = default;

    ~OpenGLShaderAdapter() override;

    auto Compile() -> void override;

   private:
    /// The OpenGL ID associated to this shader
    uint32_t m_OpenGLId = 0;
};

}  // namespace opengl
}  // namespace renderer
