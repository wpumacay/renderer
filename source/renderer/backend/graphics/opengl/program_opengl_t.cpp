#include <glad/gl.h>

#include <utils/logging.hpp>

#include <renderer/backend/graphics/opengl/program_opengl_t.hpp>

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

auto CompileShader(const char* source, eShaderType type) -> uint32_t {
    auto shader_opengl_id = glCreateShader(ToOpenGLEnum(type));
    glShaderSource(shader_opengl_id, 1, &source, nullptr);
    glCompileShader(shader_opengl_id);

    // Check for erros during compileation ---------------------------------
    int32_t compilation_success = 0;
    glGetShaderiv(shader_opengl_id, GL_COMPILE_STATUS, &compilation_success);
    if (compilation_success != GL_TRUE) {
        constexpr uint32_t ERROR_BUFFER_SIZE = 1024;
        std::array<GLchar, ERROR_BUFFER_SIZE> error_buffer{};
        glGetShaderInfoLog(shader_opengl_id, ERROR_BUFFER_SIZE, nullptr,
                           error_buffer.data());
        glDeleteShader(shader_opengl_id);
        shader_opengl_id = 0;

        LOG_CORE_ERROR(
            "Program::CompileShader > coudln't compile shader: type={0},\n"
            "error={1}",
            ::renderer::ToString(type), error_buffer.data());
    }
    return shader_opengl_id;
}

OpenGLProgram::OpenGLProgram(const char* vert_src, const char* frag_src)
    : m_VertSource(vert_src), m_FragSource(frag_src) {}

OpenGLProgram::~OpenGLProgram() {
    if (m_OpenGLId != 0) {
        glDeleteProgram(m_OpenGLId);
        m_OpenGLId = 0;
    }
}

auto OpenGLProgram::Build() -> void {
    // Compile shaders first
    const char* shader_vert_src = m_VertSource.c_str();
    const char* shader_frag_src = m_FragSource.c_str();

    auto vert_opengl_id = CompileShader(shader_vert_src, eShaderType::VERTEX);
    if (vert_opengl_id == 0) {
        return;
    }

    auto frag_opengl_id = CompileShader(shader_frag_src, eShaderType::FRAGMENT);
    if (frag_opengl_id == 0) {
        return;
    }

    // Link shaders into a single program
    m_OpenGLId = glCreateProgram();
    glAttachShader(m_OpenGLId, vert_opengl_id);
    glAttachShader(m_OpenGLId, frag_opengl_id);
    glLinkProgram(m_OpenGLId);
    glDetachShader(m_OpenGLId, vert_opengl_id);
    glDeleteShader(vert_opengl_id);
    vert_opengl_id = 0;
    glDetachShader(m_OpenGLId, frag_opengl_id);
    glDeleteShader(frag_opengl_id);
    frag_opengl_id = 0;

    int32_t linking_success = 0;
    glGetProgramiv(m_OpenGLId, GL_LINK_STATUS, &linking_success);
    if (linking_success != GL_TRUE) {
        constexpr uint32_t ERROR_BUFFER_SIZE = 1024;
        std::array<GLchar, ERROR_BUFFER_SIZE> error_buffer{};
        glGetProgramInfoLog(m_OpenGLId, ERROR_BUFFER_SIZE, nullptr,
                            error_buffer.data());
        glDeleteProgram(m_OpenGLId);
        LOG_CORE_ERROR("Program::Build> couldn't link program: \nerror{0}",
                       error_buffer.data());
        m_OpenGLId = 0;
        return;
    }
    m_IsValid = true;
}

auto OpenGLProgram::Bind() const -> void { glUseProgram(m_OpenGLId); }

// NOLINTNEXTLINE
auto OpenGLProgram::Unbind() const -> void { glUseProgram(0); }

auto OpenGLProgram::_GetUniformLocation(const char* uname) -> int32_t {
    if (m_UniformLocationsCache.find(uname) == m_UniformLocationsCache.end()) {
        m_UniformLocationsCache[uname] =
            glGetUniformLocation(m_OpenGLId, uname);
    }

    if (m_UniformLocationsCache[uname] == -1) {
        LOG_CORE_ERROR(
            "Program::_GetUniformLocation> couldn't find uniform location {0}",
            uname);
    }

    return m_UniformLocationsCache[uname];
}

auto OpenGLProgram::SetInt(const char* uname, int32_t uvalue) -> void {
    glUniform1i(_GetUniformLocation(uname), uvalue);
}

auto OpenGLProgram::SetFloat(const char* uname, float uvalue) -> void {
    glUniform1f(_GetUniformLocation(uname), uvalue);
}

auto OpenGLProgram::SetVec2(const char* uname, const Vec2& uvalue) -> void {
    glUniform2fv(_GetUniformLocation(uname), 1, uvalue.data());
}

auto OpenGLProgram::SetVec3(const char* uname, const Vec3& uvalue) -> void {
    glUniform3fv(_GetUniformLocation(uname), 1, uvalue.data());
}

auto OpenGLProgram::SetVec4(const char* uname, const Vec4& uvalue) -> void {
    glUniform4fv(_GetUniformLocation(uname), 1, uvalue.data());
}

auto OpenGLProgram::SetMat4(const char* uname, const Mat4& uvalue) -> void {
    glUniformMatrix4fv(_GetUniformLocation(uname), 1, GL_FALSE, uvalue.data());
}

}  // namespace opengl
}  // namespace renderer
