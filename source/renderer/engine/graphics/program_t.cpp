
#include <glad/gl.h>

#include <utils/logging.hpp>
#include <renderer/shader/program_t.hpp>

namespace renderer {

Program::Program(const char* name) : m_Name(name) {}

Program::Program(const char* name, const char* vert_src, const char* frag_src)
    : m_Name(name) {
    auto vert_shader = std::make_unique<Shader>((m_Name + "_vert").c_str(),
                                                eShaderType::VERTEX, vert_src);
    auto frag_shader = std::make_unique<Shader>(
        (m_Name + "_frag").c_str(), eShaderType::FRAGMENT, frag_src);

    AddShader(std::move(vert_shader));
    AddShader(std::move(frag_shader));
    LinkProgram();
}

auto Program::AddShader(Shader::uptr shader) -> void {
    if (m_NumShaders >= MAX_SHADERS_IN_PROGRAM) {
        LOG_CORE_WARN(
            "Program::AddShader >>> reached max num. of shaders in program");
        return;
    }
    m_Shaders.at(m_NumShaders++) = std::move(shader);
}

auto Program::LinkProgram() -> void {
    if (m_NumShaders < 2) {
        LOG_CORE_ERROR(
            "Shader::LinkProgram >>> There're fewer that 2 shaders linked to "
            "program {0}",
            m_Name);
        return;
    }

    for (uint32_t i = 0; i < m_NumShaders; ++i) {
        if (!m_Shaders.at(i)->compiled()) {
            LOG_CORE_ERROR(
                "Shader::LinkProgram >>> shader {0} is not compiled :c. Error "
                "thrown while linking program {1}",
                m_Shaders.at(i)->name(), m_Name);
            return;
        }
    }

    m_OpenGLId = glCreateProgram();
    for (uint32_t i = 0; i < m_NumShaders; ++i) {
        uint32_t shader_opengl_id = m_Shaders.at(i)->opengl_id();
        glAttachShader(m_OpenGLId, shader_opengl_id);
    }
    glLinkProgram(m_OpenGLId);
    for (uint32_t i = 0; i < m_NumShaders; ++i) {
        uint32_t shader_opengl_id = m_Shaders.at(i)->opengl_id();
        glDetachShader(m_OpenGLId, shader_opengl_id);
        m_Shaders.at(i)->Release();
    }

    int32_t linking_success = 0;
    glGetProgramiv(m_OpenGLId, GL_LINK_STATUS, &linking_success);
    if (linking_success != GL_TRUE) {
        constexpr uint32_t ERROR_BUFFER_SIZE = 1024;
        std::array<GLchar, ERROR_BUFFER_SIZE> error_buffer{};
        glGetProgramInfoLog(m_OpenGLId, ERROR_BUFFER_SIZE, nullptr,
                            error_buffer.data());
        glDeleteProgram(m_OpenGLId);
        LOG_CORE_ERROR(
            "Program::LinkProgram >>> couldn't link program: "
            "name={0},\nerror{1}",
            m_Name, error_buffer.data());
        m_Linked = false;
        m_OpenGLId = 0;
        return;
    }
    m_Linked = true;
}

Program::~Program() {
    for (uint32_t i = 0; i < m_NumShaders; ++i) {
        m_Shaders.at(i) = nullptr;
    }
    glDeleteProgram(m_OpenGLId);
}

auto Program::Bind() const -> void { glUseProgram(m_OpenGLId); }

// NOLINTNEXTLINE
auto Program::Unbind() const -> void { glUseProgram(0); }

auto Program::GetUniformLocation(const char* uname) -> int32_t {
    if (m_UniformLocationsCache.find(uname) == m_UniformLocationsCache.end()) {
        m_UniformLocationsCache[uname] =
            glGetUniformLocation(m_OpenGLId, uname);
    }

    if (m_UniformLocationsCache[uname] == -1) {
        LOG_CORE_ERROR(
            "Program::GetUniformLocation >>> couldn't find uniform location "
            "{0} on program {1}",
            uname, m_Name);
    }

    return m_UniformLocationsCache[uname];
}

void Program::SetInt(const char* uname, int32_t uvalue) {
    glUniform1i(GetUniformLocation(uname), uvalue);
}

void Program::SetFloat(const char* uname, float32_t uvalue) {
    glUniform1f(GetUniformLocation(uname), uvalue);
}

void Program::SetVec2(const char* uname, const Vec2& uvalue) {
    glUniform2fv(GetUniformLocation(uname), 1, uvalue.data());
}

void Program::SetVec3(const char* uname, const Vec3& uvalue) {
    glUniform3fv(GetUniformLocation(uname), 1, uvalue.data());
}

void Program::SetVec4(const char* uname, const Vec4& uvalue) {
    glUniform4fv(GetUniformLocation(uname), 1, uvalue.data());
}

void Program::SetMat4(const char* uname, const Mat4& uvalue) {
    glUniformMatrix4fv(GetUniformLocation(uname), 1, GL_FALSE,
                       uvalue[0].data());
}

}  // namespace renderer
