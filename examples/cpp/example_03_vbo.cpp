
#include <iostream>
#include <loco/renderer/core/vertex_buffer_layout_t.hpp>

auto main() -> int {
    loco::renderer::BufferLayout layout = {
        {"position", loco::renderer::eElementType::FLOAT_2, false},
        {"color", loco::renderer::eElementType::FLOAT_4, false}};

    std::cout << layout.ToString() << std::endl;

    return 0;
}
