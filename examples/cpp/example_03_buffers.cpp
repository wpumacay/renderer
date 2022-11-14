#include <array>
#include <memory>

#include <glad/gl.h>

#include <renderer/window/window_t.hpp>
#include <renderer/shader/program_t.hpp>
#include <renderer/core/vertex_buffer_t.hpp>
#include <renderer/core/vertex_array_t.hpp>

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;

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
    auto window =
        std::make_unique<renderer::Window>(WINDOW_WIDTH, WINDOW_HEIGHT);

    window->RegisterKeyboardCallback([&](int key, int, int) {
        if (key == renderer::keys::KEY_ESCAPE) {
            window->RequestClose();
        }
    });

    auto program = std::make_unique<renderer::Program>(
        "basic_2d", VERT_SHADER_SRC, FRAG_SHADER_SRC);

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

    renderer::BufferLayout layout = {
        {"position", renderer::eElementType::FLOAT_2, false},
        {"color", renderer::eElementType::FLOAT_3, false}};

    auto vbo = std::make_unique<renderer::VertexBuffer>(
        layout, renderer::eBufferUsage::STATIC,
        static_cast<uint32_t>(sizeof(buffer_data)), buffer_data);

    auto vao = std::make_unique<renderer::VertexArray>();
    vao->AddVertexBuffer(std::move(vbo));

    while (window->active()) {
        window->Begin();
        program->Bind();
        vao->Bind();

        glDrawArrays(GL_TRIANGLES, 0, NUM_VERTICES);

        vao->Unbind();
        program->Unbind();
        window->End();
    }

    return 0;
}
