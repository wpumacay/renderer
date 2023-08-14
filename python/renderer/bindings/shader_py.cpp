#include <utility>

#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/numpy.h>

#include <renderer/shader/shader_t.hpp>
#include <renderer/shader/program_t.hpp>

#include <conversions_py.hpp>

namespace py = pybind11;

namespace renderer {

// NOLINTNEXTLINE
void bindings_shader(py::module& m) {
    {
        using Enum = renderer::eShaderType;
        py::enum_<Enum>(m, "ShaderType")
            .value("VERTEX", Enum::VERTEX)
            .value("FRAGMENT", Enum::FRAGMENT)
            .value("GEOMETRY", Enum::GEOMETRY)
            .value("COMPUTE", Enum::COMPUTE)
            .value("TESSELLATION_CONTROL", Enum::TESSELLATION_CONTROL)
            .value("TESSELLATION_EVALUATION", Enum::TESSELLATION_EVALUATION);
    }

    {
        using Class = renderer::Shader;
        py::class_<Class, Class::ptr>(m, "Shader")
            .def(py::init<const char*, const eShaderType&, const char*>())
            .def_property_readonly("name", &Class::name)
            .def_property_readonly("type", &Class::type)
            .def_property_readonly("source", &Class::source)
            .def_property_readonly("opengl_id", &Class::opengl_id)
            .def_property_readonly("compiled", &Class::compiled)
            .def("__repr__", [](const Class& self) -> py::str {
                return py::
                    str("Shader(name={}, type={}, opengl_id={}, compiled={})")
                        .format(self.name(), ToString(self.type()),
                                self.opengl_id(), self.compiled());
            });
    }

    {
        using Class = renderer::Program;
        py::class_<Class, Class::ptr>(m, "Program")
            //// Don't expose default constructor. Can't assemble shaders
            //// separately as we're not exposing shader constructor
            //// .def(py::init<const char*>())
            .def(py::init<const char*, const char*, const char*>())
            //// .def(
            ////     "AddShader",
            ////     [](Class& self, Shader::uptr shader) -> void {
            ////         self.AddShader(std::move(shader));
            ////     },
            ////     py::keep_alive<1, 2>())
            //// .def("LinkProgram", &Class::LinkProgram)
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
            .def_property_readonly("name", &Class::name)
            .def_property_readonly("opengl_id", &Class::opengl_id)
            .def_property_readonly("linked", &Class::linked)
            .def_property_readonly("num_shaders", &Class::num_shaders)
            //// TODO(wilbert): add get_shader method or similar
            .def("__repr__", [](const Class& self) -> py::str {
                return py::str(
                           "Program(name={}, opengl_id={}, linked={}, "
                           "num_shaders={})")
                    .format(self.name(), self.opengl_id(), self.linked(),
                            self.num_shaders());
            });
    }
}

}  // namespace renderer
