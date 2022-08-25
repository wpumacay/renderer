// clang-format off
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/functional.h>

#include <loco/renderer/window/window.hpp>
// clang-format on

namespace py = pybind11;

namespace loco {
namespace renderer {

// NOLINTNEXTLINE
void bindings_window(py::module& m) {
    {
        using Enum = loco::renderer::eWindowBackend;
        py::enum_<Enum>(m, "eWindowBackend")
            .value("TYPE_NONE", Enum::TYPE_NONE)
            .value("TYPE_GLFW", Enum::TYPE_GLFW)
            .value("TYPE_EGL", Enum::TYPE_EGL)
            .value("TYPE_OSMESA", Enum::TYPE_OSMESA);
    }

    {
        using Class = loco::renderer::WindowProperties;
        py::class_<Class>(m, "WindowProperties")
            .def(py::init<>())
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
