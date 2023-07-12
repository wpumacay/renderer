#include <stdexcept>

#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/functional.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>

#include <renderer/core/vertex_buffer_t.hpp>
#include <renderer/core/index_buffer_t.hpp>
#include <renderer/core/vertex_array_t.hpp>

#include <conversions_py.hpp>
#include <spdlog/fmt/bundled/format.h>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#endif

namespace py = pybind11;

namespace renderer {

// NOLINTNEXTLINE
void bindings_buffers(py::module& m) {
    using NumpyFloatArray = py::array_t<float32_t>;
    using NumpyUint32Array = py::array_t<uint32_t>;

    {
        using Enum = renderer::eElementType;
        constexpr auto* EnumName = "ElementType";  // NOLINT
        py::enum_<Enum>(m, EnumName)
            .value("FLOAT_1", Enum::FLOAT_1)
            .value("FLOAT_2", Enum::FLOAT_2)
            .value("FLOAT_3", Enum::FLOAT_3)
            .value("FLOAT_4", Enum::FLOAT_4)
            .value("INT_1", Enum::INT_1)
            .value("INT_2", Enum::INT_2)
            .value("INT_3", Enum::INT_3)
            .value("INT_4", Enum::INT_4);
    }

    {
        using Class = renderer::BufferElement;
        constexpr auto* ClassName = "BufferElement";  // NOLINT
        py::class_<Class>(m, ClassName)
            .def(py::init<>())
            .def(py::init<const char*, const eElementType&, bool>())
            .def_readwrite("name", &Class::name)
            .def_readwrite("type", &Class::type)
            .def_readwrite("count", &Class::count)
            .def_readwrite("size", &Class::size)
            .def_readwrite("offset", &Class::offset)
            .def_readwrite("normalized", &Class::normalized)
            .def("__repr__", [](const Class& self) -> py::str {
                return py::str(
                           "BufferElement(name={}, type={}, count={}, size={}, "
                           "offset={}, normalized={})")
                    .format(self.name, ToString(self.type), self.count,
                            self.size, self.offset, self.normalized);
            });
    }

    {
        using Class = renderer::BufferLayout;
        constexpr auto* ClassName = "BufferLayout";  // NOLINT
        py::class_<Class>(m, ClassName)
            .def(py::init<>())
            .def(py::init([](const py::list& list_elements) {  // NOLINT
                std::vector<BufferElement> buffer_elements;
                for (const auto& py_element : list_elements) {
                    if (!py::isinstance<py::list>(py_element)) {
                        throw std::runtime_error(
                            "BufferLayout >>> must pass a list of buffer "
                            "elements to constructor");
                    }

                    auto list_element =
                        py::reinterpret_borrow<py::list>(py_element);

                    constexpr size_t NUM_BUFF_ELEM_PARAMS = 3;
                    if (list_element.size() != NUM_BUFF_ELEM_PARAMS) {
                        throw std::runtime_error(
                            "BufferLayout >>> elements from list must contain "
                            "3 parameters");
                    }

                    if (!py::isinstance<py::str>(list_element[0]) ||
                        !py::isinstance<eElementType>(list_element[1]) ||
                        !py::isinstance<py::bool_>(list_element[2])) {
                        throw std::runtime_error(
                            "BufferLayout >>> elements from list must contain "
                            "types (str,eElementType,bool)");
                    }

                    // NOLINTNEXTLINE
                    auto elm_name = (std::string)py::str(list_element[0]);
                    auto elm_type = py::cast<eElementType>(list_element[1]);
                    auto elm_norm = py::bool_(list_element[2]);

                    buffer_elements.emplace_back(elm_name.c_str(), elm_type,
                                                 elm_norm);
                }

                return std::make_unique<BufferLayout>(buffer_elements);
            }))
            .def_property_readonly("stride", &Class::stride)
            .def_property_readonly(
                "elements",
                [](const Class& self) -> std::vector<BufferElement> {
                    return self.elements();
                })
            .def("__repr__",
                 [](const Class& self) -> py::str { return self.ToString(); });
    }

    {
        using Enum = renderer::eBufferUsage;
        constexpr auto* EnumName = "BufferUsage";  // NOLINT
        py::enum_<Enum>(m, EnumName)
            .value("STATIC", Enum::STATIC)
            .value("DYNAMIC", Enum::DYNAMIC);
    }

    {
        using Class = renderer::VertexBuffer;
        constexpr auto* ClassName = "VertexBuffer";  // NOLINT
        py::class_<Class, Class::ptr>(m, ClassName)
            // NOLINTNEXTLINE
            .def(py::init([](BufferLayout layout, eBufferUsage usage,
                             uint32_t size, const NumpyFloatArray& data) {
                if (size !=
                    (static_cast<size_t>(data.size()) * sizeof(float32_t))) {
                    throw std::runtime_error(
                        fmt::format("VertexBuffer >>> given size must match "
                                    "the size of the given numpy-array. Given "
                                    "size={0}, np-array size={1}",
                                    size, data.size()));
                }

                return std::make_unique<VertexBuffer>(
                    layout, usage, size,
                    static_cast<const float32_t*>(data.request().ptr));
            }))
            .def("resize", &Class::Resize)
            .def("updateData",
                 [](Class& self, uint32_t size, const NumpyFloatArray& data) {
                     self.UpdateData(size, static_cast<const float32_t*>(
                                               data.request().ptr));
                 })
            .def("bind", &Class::Bind)
            .def("unbind", &Class::Unbind)
            .def_property_readonly("layout", &Class::layout)
            .def_property_readonly("size", &Class::size)
            .def_property_readonly("usage", &Class::usage)
            .def_property_readonly("opengl_id", &Class::opengl_id)
            .def("__repr__",
                 [](const Class& self) -> py::str { return self.ToString(); });
    }

    {
        using Class = renderer::IndexBuffer;
        constexpr auto* ClassName = "IndexBuffer";  // NOLINT
        py::class_<Class, Class::ptr>(m, ClassName)
            .def(py::init([](eBufferUsage usage, uint32_t count,  // NOLINT
                             const NumpyUint32Array& data) {
                return std::make_unique<Class>(
                    usage, count,
                    static_cast<const uint32_t*>(data.request().ptr));
            }))
            .def("bind", &Class::Bind)
            .def("unbind", &Class::Unbind)
            .def_property_readonly("count", &Class::count)
            .def_property_readonly("opengl_id", &Class::opengl_id)
            .def("__repr__",
                 [](const Class& self) -> py::str { return self.ToString(); });
    }

    {
        using Class = renderer::VertexArray;
        constexpr auto* ClassName = "VertexArray";  // NOLINT
        py::class_<Class>(m, ClassName)
            .def(py::init<>())
            .def("addVertexBuffer", &Class::AddVertexBuffer)
            .def("setIndexBuffer", &Class::SetIndexBuffer)
            .def("bind", &Class::Bind)
            .def("unbind", &Class::Unbind)
            .def_property_readonly("opengl_id", &Class::opengl_id)
            .def_property_readonly("num_attribs", &Class::num_attribs)
            .def_property_readonly("num_buffers", &Class::num_buffers)
            .def("__repr__",
                 [](const Class& self) -> py::str { return self.ToString(); });
    }
}

}  // namespace renderer

#if defined(__clang__)
#pragma clang diagnostic pop  // NOLINT
#endif
