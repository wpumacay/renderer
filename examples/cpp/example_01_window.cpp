
#include <renderer/window/window_t.hpp>
#include <utils/logging.hpp>

constexpr int DEFAULT_WIDTH = 1024;
constexpr int DEFAULT_HEIGHT = 768;

auto main() -> int {
    renderer::WindowConfig win_config;
    win_config.backend = renderer::eWindowBackend::TYPE_GLFW;
    win_config.width = DEFAULT_WIDTH;
    win_config.height = DEFAULT_HEIGHT;
    win_config.title = "Example 01 - Window";

    auto window = std::make_unique<renderer::Window>(win_config);
    window->RegisterKeyboardCallback([&](int key, int action, int mods) {
        LOG_TRACE("key: {0}, action: {1}, mods: {2}", key, action, mods);
        if ((mods & renderer::MOD_SHIFT) != 0) {
            LOG_INFO("Holding SHIFT key");
        }
        if ((mods & renderer::MOD_CONTROL) != 0) {
            LOG_INFO("Holding CONTROL key");
        }
        if ((mods & renderer::MOD_ALT) != 0) {
            LOG_INFO("Holding ALT key");
        }

        if (key == renderer::keys::KEY_ESCAPE) {
            window->RequestClose();
        }
    });

    window->RegisterScrollCallback([](double xOff, double yOff) {
        LOG_TRACE("xOff: {0}, yOff: {1}", xOff, yOff);
    });

    while (window->active()) {
        window->Begin();
        // opengl-stuff can go here :)
        window->End();
    }

    return 0;
}
