#include <chrono>

#include <bits/chrono.h>
#include <glad/gl.h>

#include <renderer/window/window_t.hpp>
#include <renderer/assets/shader_manager_t.hpp>
#include <renderer/assets/texture_manager_t.hpp>
#include <renderer/input/input_manager_t.hpp>
#include <renderer/core/vertex_buffer_layout_t.hpp>
#include <renderer/core/vertex_buffer_t.hpp>
#include <renderer/core/index_buffer_t.hpp>
#include <renderer/core/vertex_array_t.hpp>

#include <utils/logging.hpp>
#include "renderer/input/buttons.hpp"

#if defined(RENDERER_IMGUI)
#include <imgui.h>
#endif  // RENDERER_IMGUI

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;

namespace renderer {

struct Engine {
    Window::uptr window = nullptr;
    ShaderManager::uptr shader_manager = nullptr;
    TextureManager::uptr texture_manager = nullptr;
    InputManager::uptr input_manager = nullptr;
    std::chrono::steady_clock::time_point stamp_now;
    std::chrono::steady_clock::time_point stamp_bef;
    double time = 0.0;
    double delta = 0.0;
};

}  // namespace renderer

// NOLINTNEXTLINE
renderer::Engine g_engine;

auto main() -> int {
    g_engine.window =
        std::make_unique<renderer::Window>(WINDOW_WIDTH, WINDOW_HEIGHT);
    g_engine.shader_manager = std::make_unique<renderer::ShaderManager>();
    g_engine.texture_manager = std::make_unique<renderer::TextureManager>();
    g_engine.input_manager = std::make_unique<renderer::InputManager>();
    g_engine.window->RegisterKeyboardCallback([&](int key, int action, int) {
        g_engine.input_manager->CallbackKey(key, action);
    });
    g_engine.window->RegisterMouseButtonCallback(
        [&](int button, int action, double x, double y) {
            g_engine.input_manager->CallbackMouseButton(
                button, action, static_cast<float>(x), static_cast<float>(y));
        });
    g_engine.window->RegisterMouseMoveCallback([&](double x, double y) {
        g_engine.input_manager->CallbackMouseMove(static_cast<float>(x),
                                                  static_cast<float>(y));
    });
    g_engine.window->RegisterScrollCallback([&](double xOff, double yOff) {
        g_engine.input_manager->CallbackScroll(static_cast<float>(xOff),
                                               static_cast<float>(yOff));
    });

    auto program = g_engine.shader_manager->LoadProgram(
        "basic2d",
        renderer::EXAMPLES_PATH + "/resources/shaders/basic2d_vert.glsl",
        renderer::EXAMPLES_PATH + "/resources/shaders/basic2d_frag.glsl");

    auto texture = g_engine.texture_manager->LoadTexture(
        "container",
        renderer::EXAMPLES_PATH + "/resources/images/container.jpg");

    // -------------------------------------------------------------------------
    // Setup the primitive to be drawn

    // clang-format off
    // NOLINTNEXTLINE
    float buffer_data[] = {
    /*|      pos       texture  */
        -0.5F, -0.5F, 0.0F, 0.0F, // NOLINT
         0.5F, -0.5F, 2.0F, 0.0F, // NOLINT
         0.5F,  0.5F, 2.0F, 2.0F, // NOLINT
        -0.5F,  0.5F, 0.0F, 2.0F // NOLINT
    };
    constexpr uint32_t NUM_VERTICES = 6;

    // NOLINTNEXTLINE
    uint32_t buffer_indices[] = {
        0, 1, 2,  // first triangle
        0, 2, 3  // second triangle
    };
    // clang-format on

    renderer::BufferLayout layout = {
        {"position", renderer::eElementType::FLOAT_2, false},
        {"texcoord", renderer::eElementType::FLOAT_2, false}};

    auto vbo = std::make_unique<renderer::VertexBuffer>(
        layout, renderer::eBufferUsage::STATIC,
        static_cast<uint32_t>(sizeof(buffer_data)), buffer_data);

    auto ibo = std::make_unique<renderer::IndexBuffer>(
        renderer::eBufferUsage::STATIC, NUM_VERTICES, buffer_indices);

    auto vao = std::make_unique<renderer::VertexArray>();
    vao->AddVertexBuffer(std::move(vbo));
    vao->SetIndexBuffer(std::move(ibo));
    // -------------------------------------------------------------------------

    program->Bind();
    Vec3 color = {0.2F, 0.4F, 0.8F};  // NOLINT
    float radius = 0.1F;              // NOLINT
    program->SetFloat("u_time", 0.0F);
    program->SetFloat("u_radius", radius);
    program->SetVec3("u_color", color);
    program->Unbind();

    g_engine.stamp_bef = std::chrono::steady_clock::now();
    g_engine.stamp_now = std::chrono::steady_clock::now();

    while (g_engine.window->active()) {
        g_engine.window->Begin();

        if (g_engine.input_manager->IsKeyDown(renderer::keys::KEY_ESCAPE)) {
            g_engine.window->RequestClose();
        }

        auto cursor_position = g_engine.input_manager->GetCursorPosition();
        LOG_INFO("Cursorposition: ({0},{1})", cursor_position.x(),
                 cursor_position.y());
        LOG_INFO("MouseButtonLeft: {0}", g_engine.input_manager->IsMouseDown(
                                             renderer::mouse::BUTTON_LEFT));
        LOG_INFO("MouseButtonMiddle: {0}", g_engine.input_manager->IsMouseDown(
                                               renderer::mouse::BUTTON_MIDDLE));
        LOG_INFO("MouseButtonRight: {0}", g_engine.input_manager->IsMouseDown(
                                              renderer::mouse::BUTTON_RIGHT));

        // Draw the quad
        {
            program->Bind();
            texture->Bind();
            vao->Bind();

            // Update our uniforms
            program->SetFloat("u_time", static_cast<float>(g_engine.time));
            program->SetFloat("u_radius", radius);
            program->SetVec3("u_color", color);

            glDrawElements(GL_TRIANGLES, NUM_VERTICES, GL_UNSIGNED_INT,
                           nullptr);

            vao->Unbind();
            texture->Unbind();
            program->Unbind();
        }
#if defined(RENDERER_IMGUI)
        ImGui::Begin("Demo options");
        {
            // NOLINTNEXTLINE
            ImGui::Text("time: %.5f, delta: %.5f", g_engine.time,
                        g_engine.delta);
            ImGui::SliderFloat("radius", &radius, -0.2F, 0.2F);  // NOLINT
            ImGui::ColorEdit3("color", color.data());
        }
#endif  // RENDERER_IMGUI

        g_engine.window->End();

        // Keep track of time
        {
            g_engine.stamp_now = std::chrono::steady_clock::now();
            g_engine.delta =
                std::chrono::duration_cast<std::chrono::duration<double>>(
                    g_engine.stamp_now - g_engine.stamp_bef)
                    .count();
            g_engine.time += g_engine.delta;
            g_engine.stamp_bef = g_engine.stamp_now;
        }
    }

    return 0;
}
