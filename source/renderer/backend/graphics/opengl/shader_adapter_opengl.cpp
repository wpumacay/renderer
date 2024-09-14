#include <array>

#include <glad/gl.h>

#include <utils/logging.hpp>

#include <renderer/backend/graphics/opengl/shader_adapter_opengl.hpp>
#include <renderer/engine/graphics/shader_t.hpp>

namespace renderer {
namespace opengl {

auto ToOpenGLEnum(eShaderType type) -> uint32_t {
    switch (type) {
        case eShaderType::VERTEX:
            return GL_VERTEX_SHADER;
        case eShaderType::FRAGMENT:
          return GL_FRAGMENT_SHADER;
        case eShaderType::GEOMETRY:
          return GL_GEOMETRY_SHADER;
        case eShaderType::COMPUTE:
            return GL_COMPUTE_SHADER;
        default:
            return GL_VERTEX_SHADER;
    }
}

OpenGLShaderAdapter::~OpenGLShaderAdapter() {
    if (m_OpenGLId != 0) {
        glDeleteShader(m_OpenGLId);
        m_OpenGLId = 0;
    }
}


auto OpenGLShaderAdapter::Compile() -> void {
    if (auto shader_ref = m_ShaderHandle.lock()) {
        auto shader_type = shader_ref->type();
        auto shader_source = shader_ref->source();
        const char* shader_source_chr = shader_source.c_str();

        m_OpenGLId = glCreateShader(ToOpenGLEnum(shader_type));
        glShaderSource(m_OpenGLId, 1, &shader_source_chr, nullptr);
        glCompileShader(m_OpenGLId);

        // Check for erros during compileation ---------------------------------
        int32_t compilation_success = 0;
        glGetShaderiv(m_OpenGLId, GL_COMPILE_STATUS, &compilation_success);
        if (compilation_success != GL_TRUE) {
            constexpr uint32_t ERROR_BUFFER_SIZE = 1024;
            std::array<GLchar, ERROR_BUFFER_SIZE> error_buffer{};
            glGetShaderInfoLog(m_OpenGLId, ERROR_BUFFER_SIZE, nullptr,
                               error_buffer.data());
            glDeleteShader(m_OpenGLId);

            LOG_CORE_ERROR(
                "Shader::Compile >>> coudln't compile shader: type={0},\n"
                "error={1}",
                ::renderer::ToString(shader_type), error_buffer.data());
            m_IsValid = false;
            m_OpenGLId = 0;
            return;
        }
        m_IsValid = true;
        // ---------------------------------------------------------------------
    }
}

}  // namespace opengl
}  // namespace renderer
