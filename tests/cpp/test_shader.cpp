#include <catch2/catch.hpp>

#include <renderer/window/window_t.hpp>
#include <renderer/shader/shader_t.hpp>

TEST_CASE("Shader class (shader_t) type", "[shader_t]") {
    // Create a window to have a valid OpenGL context
    auto window = std::make_unique<::renderer::Window>(
        800, 600, ::renderer::eWindowBackend::TYPE_GLFW);

    SECTION("Constructor - from good source") {
        constexpr const char* GOOD_VERTEX_SHADER_SRC = R"(
            # version 330 core

            layout (location = 0) in vec2 pos;
            layout (location = 1) in vec3 color;

            out vec3 frag_color;

            void main() {
                gl_Position = vec4(pos, 0.0f, 1.0f);
                frag_color = color;
            }
        )";

        auto shader = std::make_shared<::renderer::Shader>(
            "test_basic_2d", ::renderer::eShaderType::VERTEX,
            GOOD_VERTEX_SHADER_SRC);

        REQUIRE(shader->name() == "test_basic_2d");
        REQUIRE(shader->type() == ::renderer::eShaderType::VERTEX);
        REQUIRE(shader->source() == GOOD_VERTEX_SHADER_SRC);
        REQUIRE(shader->opengl_id() > 0);
        REQUIRE(shader->compiled());
    }

    SECTION("Constructor - from bad source") {
        constexpr const char* BAD_VERTEX_SHADER_SRC = R"(
            # version 330 core

            varying in vec2 pos
            varying in vec3 color

            out frag_color;

            void main() {
                gl_Position = pos;
                frag_color = color;
            }
        )";

        auto shader = std::make_shared<::renderer::Shader>(
            "test_basic_2d", ::renderer::eShaderType::VERTEX,
            BAD_VERTEX_SHADER_SRC);

        REQUIRE(shader->name() == "test_basic_2d");
        REQUIRE(shader->type() == ::renderer::eShaderType::VERTEX);
        REQUIRE_FALSE(shader->opengl_id() > 0);
        REQUIRE_FALSE(shader->compiled());
    }
}
