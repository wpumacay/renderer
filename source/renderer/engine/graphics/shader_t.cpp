#include <renderer/engine/graphics/shader_t.hpp>

#include <renderer/engine/graphics/shader_adapter_t.hpp>
#include <renderer/backend/graphics/opengl/shader_adapter_opengl.hpp>

namespace renderer {

Shader::Shader(eShaderType type, const char* source, eGraphicsAPI api)
    : m_API(api), m_Type(type), m_Source(source) {}

auto Shader::CreateShader(eShaderType type, const char* source,
                          eGraphicsAPI api) -> std::shared_ptr<Shader> {
    auto shader = std::make_shared<Shader>(type, source, api);
    shader->Initialize();
    return shader;
}

auto Shader::Initialize() -> void {
    _InitializeBackend();
}

auto Shader::_InitializeBackend() -> void {
    switch (m_API) {
        case eGraphicsAPI::OPENGL: {
            m_BackendAdapter = std::make_unique<opengl::OpenGLShaderAdapter>();
            break;
        }
        default:
            break;
    }

    if (m_BackendAdapter) {
        m_BackendAdapter->SetShaderHandle(shared_from_this());
    }
}

auto Shader::Compile() -> void {
    if (m_BackendAdapter) {
        m_BackendAdapter->Compile();
    }
}

auto Shader::IsValid() const -> bool {
    if (m_BackendAdapter) {
        return m_BackendAdapter->IsValid();
    }
    return false;
}

}  // namespace renderer
