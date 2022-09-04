#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>

#include <loco/renderer/window/window.hpp>

#include <conversions_py.hpp>
#include <loco/math/common.hpp>
#include "loco/renderer/window/window_properties.hpp"

namespace py = pybind11;

namespace loco {
namespace renderer {

// NOLINTNEXTLINE
void bindings_window(py::module& m) {
    {
        using Enum = loco::renderer::eWindowBackend;
        py::enum_<Enum>(m, "WindowBackend")
            .value("TYPE_NONE", Enum::TYPE_NONE)
            .value("TYPE_GLFW", Enum::TYPE_GLFW)
            .value("TYPE_EGL", Enum::TYPE_EGL);
    }

    {
        using Class = loco::renderer::WindowProperties;
        py::class_<Class>(m, "WindowProperties")
            .def(py::init<>())
            .def_property(
                "clearColor",
                [](const Class& self) -> py::array_t<math::float32_t> {
                    return math::vec4_to_nparray<math::float32_t>(
                        self.clear_color);
                },
                [](Class& self,
                   const py::array_t<math::float32_t>& array_np) -> void {
                    self.clear_color =
                        math::nparray_to_vec4<math::float32_t>(array_np);
                })
            .def_readwrite("backend", &Class::backend)
            .def_readwrite("width", &Class::width)
            .def_readwrite("height", &Class::height)
            .def_readwrite("active", &Class::active)
            .def_readwrite("title", &Class::title)
            .def_readwrite("gl_version_major", &Class::gl_version_major)
            .def_readwrite("gl_version_minor", &Class::gl_version_minor)
            .def("__repr__", [](const Class& self) -> py::str {
                return py::str(
                           "WindowProperties(title={}, width={}, height={}, "
                           "active={}, backend={}, gl_version_major={}, "
                           "gl_version_minor={}")
                    .format(self.title, self.width, self.height, self.active,
                            ToString(self.backend), self.gl_version_major,
                            self.gl_version_minor);
            });
    }

    {
        using Class = loco::renderer::Window;
        py::class_<Class, Class::ptr>(m, "Window")
            .def(py::init<const WindowProperties&>())
            .def(py::init<int, int, const eWindowBackend&>(), py::arg("width"),
                 py::arg("height"), py::arg("backend"))
            .def("EnableCursor", &Class::EnableCursor)
            .def("DisableCursor", &Class::DisableCursor)
            .def("Begin", &Class::Begin)
            .def("End", &Class::End)
            .def("RequestClose", &Class::RequestClose)
            .def("RegisterKeyboardCallback", &Class::RegisterKeyboardCallback)
            .def("RegisterMousePressCallback",
                 &Class::RegisterMousePressCallback)
            .def("RegisterMouseMoveCallback", &Class::RegisterMouseMoveCallback)
            .def("RegisterScrollCallback", &Class::RegisterScrollCallback)
            .def("RegisterResizeCallback", &Class::RegisterResizeCallback)
            .def_property(
                "clearColor",
                [](const Class& self) -> py::array_t<math::float32_t> {
                    return math::vec4_to_nparray<math::float32_t>(
                        self.clear_color());
                },
                [](Class& self,
                   const py::array_t<math::float32_t>& array_np) -> void {
                    self.clear_color() =
                        math::nparray_to_vec4<math::float32_t>(array_np);
                })
            .def_property_readonly("width", &Class::width)
            .def_property_readonly("height", &Class::height)
            .def_property_readonly("active", &Class::active)
            .def_property_readonly("title", &Class::title)
            .def_property_readonly("backend", &Class::backend)
            .def("properties",
                 [](const Class& self) -> WindowProperties {
                     return self.properties();
                 })
            .def("__repr__", [](const Class& self) -> py::str {
                return py::str(
                           "Window(title={}, width={}, height={}, active={}, "
                           "backend={})")
                    .format(self.title(), self.width(), self.height(),
                            self.active(), ToString(self.backend()));
            });
    }
}

}  // namespace renderer
}  // namespace loco
