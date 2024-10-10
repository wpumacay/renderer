#include <memory>

#include <renderer/engine/graphics/program_t.hpp>

#include <renderer/engine/graphics/program_adapter_t.hpp>
#include <renderer/backend/graphics/opengl/program_adapter_opengl.hpp>

namespace renderer {

Program::Program(const char* vert_src, const char* frag_src, eGraphicsAPI api)
    : m_API(api), m_VertSource(vert_src), m_FragSource(frag_src) {}

auto Program::CreateProgram(const char* vert_src, const char* frag_src,
                            eGraphicsAPI api) -> std::shared_ptr<Program> {
    auto program = std::make_shared<Program>(vert_src, frag_src, api);
    program->Initialize();
    return program;
}

auto Program::Initialize() -> void { _InitializeBackend(); }

auto Program::_InitializeBackend() -> void {
    switch (m_API) {
        case eGraphicsAPI::OPENGL: {
            m_BackendAdapter = std::make_unique<opengl::OpenGLProgramAdapter>();
            break;
        }
        default:
            break;
    }

    if (m_BackendAdapter) {
        m_BackendAdapter->SetProgramHandle(shared_from_this());
    }
}

auto Program::Build() -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->Build();
    }
}

auto Program::Bind() const -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->Bind();
    }
}

auto Program::Unbind() const -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->Unbind();
    }
}

auto Program::SetInt(const char* uname, int32_t uvalue) -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->SetInt(uname, uvalue);
    }
}

auto Program::SetFloat(const char* uname, float uvalue) -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->SetFloat(uname, uvalue);
    }
}

auto Program::SetVec2(const char* uname, const Vec2& uvalue) -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->SetVec2(uname, uvalue);
    }
}

auto Program::SetVec3(const char* uname, const Vec3& uvalue) -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->SetVec3(uname, uvalue);
    }
}

auto Program::SetVec4(const char* uname, const Vec4& uvalue) -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->SetVec4(uname, uvalue);
    }
}

auto Program::SetMat4(const char* uname, const Mat4& uvalue) -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->SetMat4(uname, uvalue);
    }
}

auto Program::IsValid() const -> bool {
    if (m_BackendAdapter) {
        return m_BackendAdapter->IsValid();
    }
    return false;
}

}  // namespace renderer
