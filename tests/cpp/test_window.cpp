#include <catch2/catch.hpp>

#include <renderer/window/window_config_t.hpp>
#include <renderer/window/window_t.hpp>

TEST_CASE("Window class (window_t) type", "[window_t]") {
    SECTION("Constructor 1 - from config") {
        ::renderer::WindowConfig config;
        config.backend = ::renderer::eWindowBackend::TYPE_GLFW;
        config.width = 800;
        config.height = 600;
        config.title = "Test Window Constructors - from config";
        config.gl_version_major = 4;
        config.gl_version_minor = 3;

        auto window = std::make_unique<::renderer::Window>(config);
        REQUIRE(window->active());
        REQUIRE(window->backend() == ::renderer::eWindowBackend::TYPE_GLFW);
        REQUIRE(window->width() == 800);
        REQUIRE(window->height() == 600);
        REQUIRE(window->title() == "Test Window Constructors - from config");
        REQUIRE(window->clear_color() == config.clear_color);
    }

    SECTION("Constructor 2 - from args") {
        auto window = std::make_unique<::renderer::Window>(
            800, 600, ::renderer::eWindowBackend::TYPE_GLFW);

        REQUIRE(window->active());
        REQUIRE(window->backend() == ::renderer::eWindowBackend::TYPE_GLFW);
        REQUIRE(window->width() == 800);
        REQUIRE(window->height() == 600);
        REQUIRE(window->title() == "Application Window");
        REQUIRE(window->clear_color() == Vec4(0.2F, 0.2F, 0.2F, 1.0F));
    }
}
