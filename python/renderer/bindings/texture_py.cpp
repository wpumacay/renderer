#include <stdexcept>

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <utils/logging.hpp>

#include <renderer/core/texture_t.hpp>

namespace py = pybind11;

namespace renderer {

auto bindings_texture(py::module& m) -> void {
    {
        using Enum = ::renderer::eTextureFormat;
        constexpr auto* EnumName = "TextureFormat";  // NOLINT
        py::enum_<Enum>(m, EnumName)
            .value("RGB", Enum::RGB)
            .value("RGBA", Enum::RGBA)
            .value("BGRA", Enum::BGRA)
            .value("DEPTH", Enum::DEPTH)
            .value("STENCIL", Enum::STENCIL);
    }

    {
        using Enum = ::renderer::eStorageType;
        constexpr auto* EnumName = "StorageType";  // NOLINT
        py::enum_<Enum>(m, EnumName)
            .value("UINT_8", Enum::UINT_8)
            .value("UINT_32", Enum::UINT_32)
            .value("FLOAT_32", Enum::FLOAT_32);
    }

    {
        using Class = ::renderer::TextureData;
        constexpr auto* ClassName = "TextureData";  // NOLINT
        py::class_<Class, Class::ptr>(m, ClassName)
            .def(py::init<const char*>())
            .def_property_readonly("width", &Class::width)
            .def_property_readonly("height", &Class::height)
            .def_property_readonly("channels", &Class::channels)
            .def_property_readonly("image_path", &Class::image_path)
            .def_property_readonly("format", &Class::format)
            .def_property_readonly("storage", &Class::storage)
            .def("numpy",
                 [](Class& self) -> py::array_t<uint8_t> {
                     auto width = self.width();
                     auto height = self.height();
                     auto depth = self.channels();

                     return py::array_t<uint8_t>({height, width, depth},
                                                 {width * depth, depth, 1},
                                                 self.data(), py::cast(self));
                 })
            .def("__repr__",
                 [](const Class& self) -> py::str { return self.ToString(); });
    }

    m.def("check_image_shape",
          [](const py::array_t<uint8_t>& np_image) -> void {
              auto info = np_image.request();
              for (size_t i = 0; i < info.shape.size(); ++i) {
                  LOG_CORE_INFO("shape-({0}) = {1}", i, info.shape[i]);
              }
              for (size_t i = 0; i < info.strides.size(); ++i) {
                  LOG_CORE_INFO("stride-({0}) = {1}", i, info.strides[i]);
              }
          });
}

}  // namespace renderer
