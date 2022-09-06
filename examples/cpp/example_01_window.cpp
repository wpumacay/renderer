
#include <loco/renderer/window/window_t.hpp>
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
    window->RegisterKeyboardCallback([&](int key, int action, int mods) {
        LOG_TRACE("key: {0}, action: {1}, mods: {2}", key, action, mods);
        if ((mods & loco::renderer::MOD_SHIFT) != 0) {
            LOG_INFO("Holding SHIFT key");
        }
        if ((mods & loco::renderer::MOD_CONTROL) != 0) {
            LOG_INFO("Holding CONTROL key");
        }
        if ((mods & loco::renderer::MOD_ALT) != 0) {
            LOG_INFO("Holding ALT key");
        }

        if (key == loco::renderer::keys::KEY_ESCAPE) {
            window->RequestClose();
        }
    });

    while (window->active()) {
        window->Begin();
        // opengl-stuff can go here :)
        window->End();
    }

    return 0;
}
