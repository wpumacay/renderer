#include <memory>
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
            .def(py::init([](const py::array_t<uint8_t>& np_data)
                              -> Class::ptr {
                auto info = np_data.request();
                if (info.ndim < 3) {
                    throw std::runtime_error(
                        "TextureData > numpy constructor only supports RGB and "
                        "RGBA images for now");
                }

                auto width = info.shape[1];
                auto height = info.shape[0];
                auto channels = info.shape[2];

                return std::make_shared<::renderer::TextureData>(
                    width, height, channels,
                    static_cast<const uint8_t*>(info.ptr));
            }))
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

    {
        using Enum = ::renderer::eTextureWrap;
        constexpr auto* EnumName = "TextureWrap";  // NOLINT
        py::enum_<Enum>(m, EnumName)
            .value("REPEAT", Enum::REPEAT)
            .value("REPEAT_MIRROR", Enum::REPEAT_MIRROR)
            .value("CLAMP_TO_EDGE", Enum::CLAMP_TO_EDGE)
            .value("CLAMP_TO_BORDER", Enum::CLAMP_TO_BORDER);
    }

    {
        using Enum = ::renderer::eTextureFilter;
        constexpr auto* EnumName = "TextureFilter";  // NOLINT
        py::enum_<Enum>(m, EnumName)
            .value("NEAREST", Enum::NEAREST)
            .value("LINEAR", Enum::LINEAR)
            .value("NEAREST_MIPMAP_NEAREST", Enum::NEAREST_MIPMAP_NEAREST)
            .value("LINEAR_MIPMAP_NEAREST", Enum::LINEAR_MIPMAP_NEAREST)
            .value("NEAREST_MIPMAP_LINEAR", Enum::NEAREST_MIPMAP_LINEAR)
            .value("LINEAR_MIPMAP_LINEAR", Enum::LINEAR_MIPMAP_LINEAR);
    }

    {
        using Enum = ::renderer::eTextureIntFormat;
        constexpr auto* EnumName = "TextureIntFormat";  // NOLINT
        py::enum_<Enum>(m, EnumName)
            .value("RED", Enum::RED)
            .value("RG", Enum::RG)
            .value("RGB", Enum::RGB)
            .value("RGBA", Enum::RGBA)
            .value("DEPTH", Enum::DEPTH)
            .value("DEPTH_STENCIL", Enum::DEPTH_STENCIL);
    }

    {
        using Class = ::renderer::Texture;
        constexpr auto* ClassName = "Texture";  // NOLINT
        py::class_<Class, Class::ptr>(m, ClassName)
            .def(py::init([](const char* tex_filepath) -> Class::ptr {
                return std::make_shared<Class>(tex_filepath);
            }))
            .def(py::init([](TextureData::ptr tex_data) -> Class::ptr {
                return std::make_shared<Class>(std::move(tex_data));
            }))
            .def("Bind", &Class::Bind)
            .def("Unbind", &Class::Unbind)
            .def_property("border_color", &Class::border_color,
                          &Class::SetBorderColor)
            .def_property("min_filter", &Class::min_filter,
                          &Class::SetMinFilter)
            .def_property("mag_filter", &Class::mag_filter,
                          &Class::SetMagFilter)
            .def_property("wrap_mode_u", &Class::wrap_mode_u,
                          &Class::SetWrapModeU)
            .def_property("wrap_mode_v", &Class::wrap_mode_v,
                          &Class::SetWrapModeV)
            .def_property_readonly("internal_format", &Class::internal_format)
            .def("texture_data", &Class::texture_data)
            .def("__repr__",
                 [](const Class& self) -> py::str { return self.ToString(); });
    }
}

}  // namespace renderer
