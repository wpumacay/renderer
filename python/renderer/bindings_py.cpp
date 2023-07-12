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

    // -------------------------------------------------------------------------
    // TODO(wilbert): replace usage of unique_ptr for shared_ptr, as we'll be
    // changing to PyBind11 from upstream, which doesn't handle that kind of
    // ownership (unlike drake's fork :C)
    //// renderer::bindings_buffers(py_module);
    // -------------------------------------------------------------------------
}
