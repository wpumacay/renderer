#include <memory>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <conversions_py.hpp>

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
            .def(py::init([](const py::array_t<float32_t>& np_position,
                             const py::array_t<float32_t>& np_target,
                             const py::array_t<float32_t>& np_worldUp) {
                     return std::make_shared<Class>(
                         ::math::nparray_to_vec3<float32_t>(np_position),
                         ::math::nparray_to_vec3<float32_t>(np_target),
                         ::math::nparray_to_vec3<float32_t>(np_worldUp));
                 }),
                 py::arg("position"), py::arg("target"), py::arg("worldUp"))
            .def("SetPosition", &Class::SetPosition, py::arg("position"))
            .def(
                "SetPosition",
                [](Class& self, const py::array_t<float32_t>& np_position) {
                    self.SetPosition(
                        ::math::nparray_to_vec3<float32_t>(np_position));
                },
                py::arg("np_position"))
            .def("SetPosition",
                 [](Class& self, float32_t x, float32_t y, float32_t z) {
                     self.SetPosition(Vec3(x, y, z));
                 })
            .def("SetOrientation", &Class::SetOrientation,
                 py::arg("quaternion"))
            .def(
                "SetOrientation",
                [](Class& self, const py::array_t<float32_t>& np_quat) {
                    self.SetOrientation(
                        ::math::nparray_to_quat<float32_t>(np_quat));
                },
                py::arg("np_quaternion"))
            .def("SetTarget", &Class::SetTarget, py::arg("target"))
            .def(
                "SetTarget",
                [](Class& self, const py::array_t<float32_t>& np_target) {
                    self.SetTarget(
                        ::math::nparray_to_vec3<float32_t>(np_target));
                },
                py::arg("np_target"))
            .def("SetProjectionData", &Class::SetProjectionData,
                 py::arg("proj_data"))
            .def("SetProjectionType", &Class::SetProjectionType,
                 py::arg("proj_type"))
            .def("SetFOV", &Class::SetFOV, py::arg("fov"))
            .def("SetAspectRatio", &Class::SetAspectRatio, py::arg("aspect"))
            .def("SetWidth", &Class::SetWidth, py::arg("width"))
            .def("SetHeight", &Class::SetHeight, py::arg("height"))
            .def("SetZNear", &Class::SetZNear, py::arg("near"))
            .def("SetZFar", &Class::SetZFar, py::arg("far"))
            .def("SetZoom", &Class::SetZoom, py::arg("zoom"))
            .def("UpdateViewMatrix", &Class::UpdateViewMatrix)
            .def("UpdateProjectionMatrix", &Class::UpdateProjectionMatrix)
            .def("ComputeBasisVectors", &Class::ComputeBasisVectors)
            .def("ComputeBasisVectorsFromOrientation",
                 &Class::ComputeBasisVectorsFromOrientation)
            .def("position", &Class::position)
            .def("orientation", &Class::orientation)
            .def("target", &Class::target)
            .def("zoom", &Class::zoom)
            .def("view_matrix", &Class::view_matrix)
            .def("proj_matrix", &Class::proj_matrix)
            .def("front", &Class::front)
            .def("up", &Class::up)
            .def("right", &Class::right)
            .def("world_up", &Class::world_up)
            .def("__repr__",
                 [](const Class& self) -> py::str { return self.ToString(); });
    }
}

}  // namespace renderer
