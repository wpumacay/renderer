#include <memory>
#include <stdexcept>

#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <renderer/backend/graphics/opengl/vertex_buffer_layout_opengl_t.hpp>
#include <renderer/backend/graphics/opengl/vertex_buffer_opengl_t.hpp>
#include <renderer/backend/graphics/opengl/vertex_array_opengl_t.hpp>

#include <conversions_py.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wshorten-64-to-32"
#pragma clang diagnostic ignored "-Wconversion"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wconversion"
#endif

namespace py = pybind11;

namespace renderer {
namespace opengl {

// NOLINTNEXTLINE
void bindings_buffers(py::module m) {
    using NumpyFloatArray = py::array_t<float32_t>;

    {
        using Class = ::renderer::opengl::OpenGLBufferElement;
        constexpr auto* ClassName = "OpenGLBufferElement";  // NOLINT
        py::class_<Class>(m, ClassName)
            .def(py::init<>())
            .def(py::init<const char*, const eElementType&, bool>())
            .def_readwrite("name", &Class::name)
            .def_readwrite("type", &Class::type)
            .def_readwrite("count", &Class::count)
            .def_readwrite("nbytes", &Class::nbytes)
            .def_readwrite("offset", &Class::offset)
            .def_readwrite("normalized", &Class::normalized)
            .def("__repr__", &Class::ToString);
    }

    {
        using Class = ::renderer::opengl::OpenGLBufferLayout;
        constexpr auto* ClassName = "OpenGLBufferLayout";  // NOLINT
        py::class_<Class>(m, ClassName)
            .def(py::init<>())
            .def("AddElement", &Class::AddElement)
            .def_property_readonly("size", &Class::size)
            .def_property_readonly("stride", &Class::stride)
            .def("__len__",
                 [](const Class& self) -> size_t { return self.size(); })
            .def("__getitem__",
                 [](Class& self, size_t index) -> OpenGLBufferElement {
                     return self[index];
                 })
            .def("__repr__", &Class::ToString);
    }

    {
        using Class = ::renderer::opengl::OpenGLVertexBuffer;
        constexpr auto* ClassName = "OpenGLVertexBuffer";  // NOLINT
        py::class_<Class, Class::ptr>(m, ClassName)
            .def(py::init([](OpenGLBufferLayout layout, eBufferUsage usage,
                             const NumpyFloatArray& np_data) {
                return std::make_unique<OpenGLVertexBuffer>(
                    layout, usage, np_data.size() * sizeof(float32_t),
                    static_cast<const float32_t*>(np_data.request().ptr));
            }))
            .def("Bind", &Class::Bind)
            .def("Unbind", &Class::Unbind)
            .def("Resize", &Class::Resize)
            .def("UpdateData",
                 [](Class& self, const NumpyFloatArray& np_data) -> void {
                     self.UpdateData(
                         np_data.size() * sizeof(float32_t),
                         static_cast<const float32_t*>(np_data.request().ptr));
                 })
            .def_property_readonly("layout", &Class::layout)
            .def_property_readonly("size", &Class::size)
            .def_property_readonly("usage", &Class::usage)
            .def_property_readonly("opengl_id", &Class::opengl_id)
            .def("__repr__", &Class::ToString);
    }

    {
        using Class = ::renderer::opengl::OpenGLVertexArray;
        constexpr auto* ClassName = "OpenGLVertexArray";  // NOLINT
        py::class_<Class, Class::ptr>(m, ClassName)
            .def(py::init<>())
            .def("AddVertexBuffer", &Class::AddVertexBuffer)
            .def("Bind", &Class::Bind)
            .def("Unbind", &Class::Unbind)
            .def_property_readonly("opengl_id", &Class::opengl_id)
            .def_property_readonly("num_attribs", &Class::num_attribs)
            .def("__repr__", &Class::ToString);
    }
}

}  // namespace opengl

}  // namespace renderer

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
