// clang-format off
#include <pybind11/pybind11.h>

// clang-format on

namespace py = pybind11;

namespace renderer {

extern void bindings_window(py::module& py_module);    // NOLINT
extern void bindings_shader(py::module& py_module);    // NOLINT
extern void bindings_keycodes(py::module& py_module);  // NOLINT
extern void bindings_buffers(py::module& py_module);   // NOLINT

}  // namespace renderer

// NOLINTNEXTLINE
PYBIND11_MODULE(renderer, py_module) {
    renderer::bindings_window(py_module);
    renderer::bindings_keycodes(py_module);
    renderer::bindings_shader(py_module);
    renderer::bindings_buffers(py_module);
}
