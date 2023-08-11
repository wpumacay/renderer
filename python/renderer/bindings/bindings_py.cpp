// clang-format off
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>

#include <utils/logging.hpp>

// clang-format on

namespace py = pybind11;

namespace renderer {

extern void bindings_window(py::module& py_module);    // NOLINT
extern void bindings_shader(py::module& py_module);    // NOLINT
extern void bindings_keycodes(py::module& py_module);  // NOLINT
extern void bindings_buffers(py::module& py_module);   // NOLINT

}  // namespace renderer

// NOLINTNEXTLINE
PYBIND11_MODULE(renderer_bindings, py_module) {
    try {
        py::module::import("math3d");
    } catch (py::error_already_set& e) {
        e.restore();
        LOG_CORE_ERROR(
            "Didn't find required module math3d. Won't be able to access "
            "fields that are math3d types, it will likely crash :(");
    }

    ::renderer::bindings_window(py_module);
    ::renderer::bindings_keycodes(py_module);
    ::renderer::bindings_shader(py_module);
    ::renderer::bindings_buffers(py_module);
}
