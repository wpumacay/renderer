#include <array>

#include <glad/gl.h>

#include <utils/logging.hpp>
#include <renderer/shader/shader_t.hpp>

namespace renderer {

auto ToString(const eShaderType& type) -> std::string {
    switch (type) {
        case eShaderType::VERTEX:
            return "vertex";
        case eShaderType::FRAGMENT:
            return "fragment";
        case eShaderType::GEOMETRY:
            return "geometry";
        case eShaderType::COMPUTE:
            return "compute";
        case eShaderType::TESSELLATION_CONTROL:
            return "tessellation_control";
        case eShaderType::TESSELLATION_EVALUATION:
            return "tessellation evaluation";
        default:
            return "vertex";
    }
}

auto ToOpenGLEnum(const eShaderType& type) -> uint32_t {
    switch (type) {
        case eShaderType::VERTEX:
            return GL_VERTEX_SHADER;
        case eShaderType::FRAGMENT:
            return GL_FRAGMENT_SHADER;
        case eShaderType::GEOMETRY:
            return GL_GEOMETRY_SHADER;
        case eShaderType::COMPUTE:
            return GL_COMPUTE_SHADER;
        case eShaderType::TESSELLATION_CONTROL:
            return GL_TESS_CONTROL_SHADER;
        case eShaderType::TESSELLATION_EVALUATION:
            return GL_TESS_EVALUATION_SHADER;
        default:
            return GL_VERTEX_SHADER;
    }
}

Shader::Shader(const char* name, const eShaderType& type, const char* source)
    : m_Name(name), m_Type(type), m_Source(source) {
    _CompileShader(source);
}

auto Shader::_CompileShader(const char* source) -> void {
    m_OpenGLId = glCreateShader(ToOpenGLEnum(m_Type));
    glShaderSource(m_OpenGLId, 1, &source, nullptr);
    glCompileShader(m_OpenGLId);

    int32_t compilation_success = 0;
    glGetShaderiv(m_OpenGLId, GL_COMPILE_STATUS, &compilation_success);
    if (compilation_success != GL_TRUE) {
        constexpr uint32_t ERROR_BUFFER_SIZE = 1024;
        std::array<GLchar, ERROR_BUFFER_SIZE> error_buffer{};
        glGetShaderInfoLog(m_OpenGLId, ERROR_BUFFER_SIZE, nullptr,
                           error_buffer.data());
        glDeleteShader(m_OpenGLId);
        LOG_CORE_ERROR(
            "Shader::_CompileShader >>> coudln't compile shader: name={0}, "
            "type={1},\nerror={2}",
            m_Name, ToString(m_Type), error_buffer.data());
        // TODO(wilbert): here we should throw a custom exception. For now an
        // error flag exposed through shader->compiled() should do the trick
        m_Compiled = false;
        m_OpenGLId = 0;
        return;
    }
    m_Compiled = true;
}

Shader::~Shader() { Release(); }

auto Shader::Release() -> void {
    if (m_OpenGLId != 0) {
        glDeleteShader(m_OpenGLId);
        m_OpenGLId = 0;
    }
}

}  // namespace renderer
