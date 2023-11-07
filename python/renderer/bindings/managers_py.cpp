#include <memory>

#include <pybind11/pybind11.h>

#include <renderer/input/input_manager_t.hpp>

namespace py = pybind11;

namespace renderer {

// NOLINTNEXTLINE
auto bindings_managers(py::module& m) -> void {
    {
        using Class = ::renderer::InputManager;
        constexpr auto* ClassName = "InputManager";  // NOLINT
        py::class_<Class, Class::ptr>(m, ClassName)
            .def(py::init<>())
            // Callbacks that need to be registered manually
            .def("CallbackKey", &Class::CallbackKey)
            .def("CallbackMouseButton", &Class::CallbackMouseButton)
            .def("CallbackMouseMove", &Class::CallbackMouseMove)
            .def("CallbackScroll", &Class::CallbackScroll)
            // Exposed API
            .def("IsKeyDown", &Class::IsKeyDown)
            .def("IsMouseDown", &Class::IsMouseDown)
            .def("GetCursorPosition", &Class::GetCursorPosition)
            .def("GetScrollOffX", &Class::GetScrollOffX)
            .def("GetScrollOffY", &Class::GetScrollOffY)
            .def("GetScrollAccumX", &Class::GetScrollAccumX)
            .def("GetScrollAccumY", &Class::GetScrollAccumY)
            .def("__repr__",
                 [](const Class& self) -> py::str { return self.ToString(); });
    }
}

}  // namespace renderer
