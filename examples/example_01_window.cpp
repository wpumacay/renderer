
#include <loco/renderer/window/window.hpp>
#include <loco/utils/logging.hpp>

constexpr int DEFAULT_WIDTH = 1024;
constexpr int DEFAULT_HEIGHT = 768;

auto main() -> int {
    loco::renderer::WindowProperties properties;
    properties.backend = loco::renderer::eWindowBackend::TYPE_GLFW;
    properties.width = DEFAULT_WIDTH;
    properties.height = DEFAULT_HEIGHT;
    properties.title = "Example 01 - Window";

    auto window = std::make_unique<loco::renderer::Window>(properties);
    window->RegisterKeyboardCallback([](int key, int action) {
        LOG_TRACE("key: {0}, action: {1}", key, action);
    });

    while (window->active()) {
        window->Begin();
        // opengl-stuff can go here :)
        window->End();
    }

    return 0;
}
