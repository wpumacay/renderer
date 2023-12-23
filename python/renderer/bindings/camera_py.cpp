#include <memory>

#include <pybind11/pybind11.h>

#include <renderer/camera/camera_t.hpp>

namespace py = pybind11;

namespace renderer {

// NOLINTNEXTLINE
auto bindings_camera(py::module& m) -> void {
    {
        using Enum = ::renderer::eProjectionType;
        py::enum_<Enum>(m, "ProjectionType")
            .value("PERSPECTIVE", Enum::PERSPECTIVE)
            .value("ORTHOGRAPHIC", Enum::ORTHOGRAPHIC);
    }

    {
        using Class = ::renderer::ProjectionData;
        constexpr auto* ClassName = "ProjectionData";  // NOLINT
        py::class_<Class>(m, ClassName)
            .def(py::init<>())
            .def_readwrite("projection", &Class::projection)
            .def_readwrite("fov", &Class::fov)
            .def_readwrite("aspect", &Class::aspect)
            .def_readwrite("width", &Class::width)
            .def_readwrite("height", &Class::height)
            .def_readwrite("near", &Class::near)
            .def_readwrite("far", &Class::far)
            .def("__repr__",
                 [](const Class& self) -> py::str { return self.ToString(); });
    }

    {
        using Class = ::renderer::Camera;
        constexpr auto ClassName = "Camera";  // NOLINT
        py::class_<Class, Class::ptr>(m, ClassName)
            .def(py::init<const Vec3&, const Vec3&, const Vec3&,
                          const ::renderer::ProjectionData&>())
            .def("__repr__", [](const Class& self) -> py::str {});
    }
}

}  // namespace renderer
