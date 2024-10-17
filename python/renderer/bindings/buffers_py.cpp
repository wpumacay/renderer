#include <stdexcept>

#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <renderer/engine/graphics/vertex_buffer_layout_t.hpp>

#include <conversions_py.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif

namespace py = pybind11;

namespace renderer {

// NOLINTNEXTLINE
void bindings_buffers(py::module m) {
    //// using NumpyFloatArray = py::array_t<float32_t>;

    {
        using Class = ::renderer::BufferElement;
        constexpr auto* ClassName = "BufferElement";  // NOLINT
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
        using Class = ::renderer::BufferLayout;
        constexpr auto* ClassName = "BufferLayout";  // NOLINT
        py::class_<Class>(m, ClassName)
            .def(py::init<>())
            .def("AddElement", &Class::AddElement)
            .def_property_readonly("size", &Class::size)
            .def_property_readonly("stride", &Class::stride)
            .def("__len__",
                 [](const Class& self) -> size_t { return self.size(); })
            .def("__getitem__",
                 [](Class& self, size_t index) -> BufferElement {
                     return self[index];
                 })
            .def("__repr__", &Class::ToString);
    }
}

}  // namespace renderer

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
