#include <renderer/engine/graphics/window_t.hpp>
#include <renderer/engine/graphics/program_t.hpp>
#include <renderer/engine/graphics/vertex_buffer_layout_t.hpp>

#include <glad/gl.h>

#include <utils/logging.hpp>

constexpr const char* VERT_SHADER_SRC = R"(
    #version 330 core

    layout (location = 0) in vec2 position;
    layout (location = 1) in vec3 color;

    out vec3 frag_color;

    void main() {
        gl_Position = vec4(position, 0.0f, 1.0f);
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
    LOG_INFO("Vertex Buffer example -----------------------------------\n");

    constexpr int WINDOW_WIDTH = 1024;
    constexpr int WINDOW_HEIGHT = 768;

    constexpr auto WINDOW_API = ::renderer::eWindowBackend::TYPE_GLFW;
    constexpr auto GRAPHICS_API = ::renderer::eGraphicsAPI::OPENGL;

    auto window = ::renderer::Window::CreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                                   WINDOW_API);
    window->RegisterKeyboardCallback([&](int key, int, int) {
        if (key == renderer::keys::KEY_ESCAPE) {
            window->RequestClose();
        }
    });

    auto program = ::renderer::Program::CreateProgram(
        VERT_SHADER_SRC, FRAG_SHADER_SRC, GRAPHICS_API);
    program->Build();
    if (!program->IsValid()) {
        LOG_CORE_ERROR("There was an error building the shader program");
        return 1;
    }

    // clang-format off
    // NOLINTNEXTLINE
    float buffer_data[] = {
    /*|      pos     |     color      |*/
        -0.5F, -0.5F, 1.0F, 0.0F, 0.0F,// NOLINT
         0.5F, -0.5F, 0.0F, 1.0F, 0.0F,// NOLINT
         0.5F,  0.5F, 0.0F, 0.0F, 1.0F,// NOLINT

        -0.5F, -0.5F, 1.0F, 0.0F, 0.0F, // NOLINT
         0.5F,  0.5F, 0.0F, 0.0F, 1.0F, // NOLINT
        -0.5F,  0.5F, 1.0F, 1.0F, 1.0F // NOLINT
    };
    // clang-format on
    constexpr int NUM_VERTICES = 6;

    ::renderer::BufferLayout layout = {
        {"position", ::renderer::eElementType::FLOAT_2, false},
        {"color", ::renderer::eElementType::FLOAT_3, false}};

    LOG_TRACE("Vertex Buffer Layout: \n{0}\n", layout.ToString());

    while (window->active()) {
        window->Begin();
        program->Bind();

        // glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);

        program->Unbind();
        window->End();
    }

    return 0;
}
