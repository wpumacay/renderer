#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/numpy.h>

#include <renderer/engine/graphics/program_t.hpp>

#include <conversions_py.hpp>

namespace py = pybind11;

namespace renderer {

// NOLINTNEXTLINE
void bindings_program(py::module m) {
    {
        using Class = renderer::Program;
        py::class_<Class, Class::ptr>(m, "Program")  // NOLINT
            .def_static("CreateProgram",
                        static_cast<Class::ptr (*)(const char*, const char*,
                                                   eGraphicsAPI)>(
                            ::renderer::Program::CreateProgram))
            .def("Build", &Class::Build)
            .def("Bind", &Class::Bind)
            .def("Unbind", &Class::Unbind)
            .def("SetInt", &Class::SetInt)
            .def("SetFloat", &Class::SetFloat)
            .def("SetVec2",
                 [](Class& self, const char* uname,
                    const py::array_t<math::float32_t>& uvalue) {
                     self.SetVec2(
                         uname, math::nparray_to_vec2<math::float32_t>(uvalue));
                 })
            .def("SetVec3",
                 [](Class& self, const char* uname,
                    const py::array_t<math::float32_t>& uvalue) {
                     self.SetVec3(
                         uname, math::nparray_to_vec3<math::float32_t>(uvalue));
                 })
            .def("SetVec4",
                 [](Class& self, const char* uname,
                    const py::array_t<math::float32_t>& uvalue) {
                     self.SetVec4(
                         uname, math::nparray_to_vec4<math::float32_t>(uvalue));
                 })
            .def("SetMat4",
                 [](Class& self, const char* uname,
                    const py::array_t<math::float32_t>& umat) {
                     self.SetMat4(uname,
                                  math::nparray_to_mat4<math::float32_t>(umat));
                 })
            .def_property_readonly("valid", &Class::IsValid)
            .def_property_readonly("vertex_source", &Class::vertex_source)
            .def_property_readonly("fragment_source", &Class::fragment_source)
            //// TODO(wilbert): add get_shader method or similar
            .def("__repr__", [](const Class& self) -> py::str {
                return py::str(
                           "<Program\n"
                           "  valid: {}\n"
                           ">")
                    .format(self.IsValid());
            });
    }
}

}  // namespace renderer
