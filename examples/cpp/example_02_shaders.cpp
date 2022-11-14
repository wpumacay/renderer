
#include <renderer/window/window_t.hpp>
#include <renderer/shader/shader_t.hpp>
#include <renderer/shader/program_t.hpp>
#include <loco/utils/logging.hpp>

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;

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
    auto window =
        std::make_unique<loco::renderer::Window>(WINDOW_WIDTH, WINDOW_HEIGHT);

    auto vert_shader = std::make_unique<loco::renderer::Shader>(
        "basic_2d_vert", loco::renderer::eShaderType::VERTEX, VERT_SHADER_SRC);

    auto frag_shader = std::make_unique<loco::renderer::Shader>(
        "basic_2d_frag", loco::renderer::eShaderType::FRAGMENT,
        FRAG_SHADER_SRC);

    if (vert_shader->compiled()) {
        LOG_INFO("Vertex shader successfully compiled");
    }

    if (frag_shader->compiled()) {
        LOG_INFO("Fragment shader successfully compiled");
    }

    auto program = std::make_unique<loco::renderer::Program>("basic_2d");
    program->AddShader(std::move(vert_shader));
    program->AddShader(std::move(frag_shader));
    program->LinkProgram();

    if (program->linked()) {
        LOG_INFO("Shader Program successfully linked");
    }

    while (window->active()) {
        window->Begin();

        window->End();
    }

    return 0;
}
