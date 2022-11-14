#include <chrono>

#include <bits/chrono.h>
#include <glad/gl.h>

#include <renderer/window/window_t.hpp>
#include <renderer/assets/shader_manager_t.hpp>
#include <renderer/assets/texture_manager_t.hpp>
#include <renderer/core/vertex_buffer_layout_t.hpp>
#include <renderer/core/vertex_buffer_t.hpp>
#include <renderer/core/index_buffer_t.hpp>
#include <renderer/core/vertex_array_t.hpp>

#if defined(LOCORENDERER_IMGUI)
#include <imgui.h>
#endif  // LOCORENDERER_IMGUI

constexpr int WINDOW_WIDTH = 1024;
constexpr int WINDOW_HEIGHT = 768;

namespace loco {
namespace renderer {

struct Engine {
    Window::uptr window = nullptr;
    ShaderManager::uptr shader_manager = nullptr;
    TextureManager::uptr texture_manager = nullptr;
    std::chrono::steady_clock::time_point stamp_now;
    std::chrono::steady_clock::time_point stamp_bef;
    double time = 0.0;
    double delta = 0.0;
};

}  // namespace renderer
}  // namespace loco

// NOLINTNEXTLINE
loco::renderer::Engine g_engine;

auto main() -> int {
    g_engine.window =
        std::make_unique<loco::renderer::Window>(WINDOW_WIDTH, WINDOW_HEIGHT);
    g_engine.shader_manager = std::make_unique<loco::renderer::ShaderManager>();
    g_engine.texture_manager =
        std::make_unique<loco::renderer::TextureManager>();

    auto program = g_engine.shader_manager->LoadProgram(
        "basic2d",
        loco::renderer::EXAMPLES_PATH + "/resources/shaders/basic2d_vert.glsl",
        loco::renderer::EXAMPLES_PATH + "/resources/shaders/basic2d_frag.glsl");

    auto texture = g_engine.texture_manager->LoadTexture(
        "container",
        loco::renderer::EXAMPLES_PATH + "/resources/images/container.jpg");

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

    loco::renderer::BufferLayout layout = {
        {"position", loco::renderer::eElementType::FLOAT_2, false},
        {"texcoord", loco::renderer::eElementType::FLOAT_2, false}};

    auto vbo = std::make_unique<loco::renderer::VertexBuffer>(
        layout, loco::renderer::eBufferUsage::STATIC,
        static_cast<uint32_t>(sizeof(buffer_data)), buffer_data);

    auto ibo = std::make_unique<loco::renderer::IndexBuffer>(
        loco::renderer::eBufferUsage::STATIC, NUM_VERTICES, buffer_indices);

    auto vao = std::make_unique<loco::renderer::VertexArray>();
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
#if defined(LOCORENDERER_IMGUI)
        ImGui::Begin("Demo options");
        {
            // NOLINTNEXTLINE
            ImGui::Text("time: %.5f, delta: %.5f", g_engine.time,
                        g_engine.delta);
            ImGui::SliderFloat("radius", &radius, -0.2F, 0.2F);  // NOLINT
            ImGui::ColorEdit3("color", color.data());
        }
#endif  // LOCORENDERER_IMGUI

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
