#include <catch2/catch.hpp>

#include <renderer/window/window_config_t.hpp>

TEST_CASE("WindowConfig class (window_config_t) type", "[window_config_t]") {
    SECTION("Default constructor") {
        auto config = ::renderer::WindowConfig();

        REQUIRE(config.backend == ::renderer::DEFAULT_WINDOWING_BACKEND);
        REQUIRE(config.width == ::renderer::DEFAULT_WINDOW_WIDTH);
        REQUIRE(config.height == ::renderer::DEFAULT_WINDOW_HEIGHT);
        REQUIRE(config.title == ::renderer::DEFAULT_WINDOW_TITLE);
        REQUIRE(config.clear_color == Vec4(0.2F, 0.2F, 0.2F, 1.0F));
        REQUIRE(config.gl_version_major ==
                ::renderer::DEFAULT_OPENGL_MAJOR_VERSION);
        REQUIRE(config.gl_version_minor ==
                ::renderer::DEFAULT_OPENGL_MINOR_VERSION);
    }
}
