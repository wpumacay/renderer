
#include <renderer/engine/graphics/window_t.hpp>
#include <renderer/engine/graphics/shader_t.hpp>

#include <utils/logging.hpp>

constexpr const char* VERT_SHADER_SRC = R"(
    #version 330 core

    layout (location = 0) in vec2 pos;
    layout (location = 1) in vec3 color;

    out vec3 frag_color;

    void main() {
        gl_Position = vec4(pos, 0.0f, 1.0f);
        frag_color = color;
    }
)";

constexpr const char* FRAG_SHADER_SRC = R"(
    #version 330 core

    in vec3 frag_color;
    out vec4 output_color;

    void main() {
        output_color = vec4(frag_color, 1.0f);
    }
)";

auto main() -> int {
    constexpr int WINDOW_WIDTH = 1024;
    constexpr int WINDOW_HEIGHT = 768;

    auto window_api = ::renderer::eWindowBackend::TYPE_GLFW;
    auto graphics_api = ::renderer::eGraphicsAPI::OPENGL;

    auto window = ::renderer::Window::CreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                                   window_api);

    auto vert_shader = ::renderer::Shader::CreateShader(
        ::renderer::eShaderType::VERTEX, VERT_SHADER_SRC, graphics_api);
    auto frag_shader = ::renderer::Shader::CreateShader(
        ::renderer::eShaderType::FRAGMENT, FRAG_SHADER_SRC, graphics_api);

    vert_shader->Compile();
    if (vert_shader->IsValid()) {
        LOG_INFO("Vertex shader successfully compiled");
    } else {
        LOG_ERROR("Vertex shader could not be compiled");
    }

    frag_shader->Compile();
    if (frag_shader->IsValid()) {
        LOG_INFO("Fragment shader successfully compiled");
    } else {
        LOG_ERROR("Fragment shader could not be compiled");
    }

    return 0;
}
