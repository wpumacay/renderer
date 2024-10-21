#include <chrono>

#include <renderer/engine/graphics/window_t.hpp>
#include <renderer/backend/graphics/opengl/program_opengl_t.hpp>

#include <glad/gl.h>

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
    constexpr auto WINDOW_API = ::renderer::eWindowBackend::TYPE_GLFW;

    auto window = ::renderer::Window::CreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                                   WINDOW_API);
    window->RegisterKeyboardCallback([&](int key, int, int) {
        if (key == ::renderer::keys::KEY_ESCAPE) {
            window->RequestClose();
        }
    });

    auto program = std::make_shared<::renderer::opengl::OpenGLProgram>(
        VERT_SHADER_SRC, FRAG_SHADER_SRC);
    program->Build();

    if (program->IsValid()) {
        LOG_INFO("Shader Program successfully built and ready for action");
    } else {
        LOG_ERROR("Shader Program couldn't be built");
    }

    while (window->active()) {
        window->Begin();

        window->End();
    }

    return 0;
}
