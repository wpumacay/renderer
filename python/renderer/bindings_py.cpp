// clang-format off
#include <pybind11/pybind11.h>

// clang-format on

namespace py = pybind11;

namespace loco {
namespace renderer {

extern void bindings_window(py::module& py_module);  // NOLINT

}  // namespace renderer
}  // namespace loco

// NOLINTNEXTLINE
PYBIND11_MODULE(renderer, py_module) {
    loco::renderer::bindings_window(py_module);
}
