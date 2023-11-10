#include <memory>

#include <pybind11/pybind11.h>

#include <renderer/input/input_manager_t.hpp>
#include <renderer/assets/shader_manager_t.hpp>
#include <renderer/assets/texture_manager_t.hpp>

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

    {
        using Class = ::renderer::ShaderManager;
        constexpr auto* ClassName = "ShaderManager";  // NOLINT
        py::class_<Class, Class::ptr>(m, ClassName)
            .def(py::init<>())
            .def("LoadProgram", &Class::LoadProgram)
            .def("CacheProgram", &Class::CacheProgram)
            .def("GetProgram", &Class::GetProgram)
            .def("DeleteProgram", &Class::DeleteProgram)
            .def("GetProgramByIndex", &Class::GetProgramByIndex)
            .def("GetNumPrograms", &Class::GetNumPrograms)
            .def("__getitem__",
                 [](Class& self, const std::string& prog_name) -> Program::ptr {
                     return self.GetProgram(prog_name);
                 })
            .def("__getitem__",
                 [](Class& self, uint32_t prog_idx) -> Program::ptr {
                     return self.GetProgramByIndex(prog_idx);
                 })
            .def("__len__",
                 [](const Class& self) -> uint32_t {
                     return self.GetNumPrograms();
                 })
            .def("__repr__",
                 [](const Class& self) -> py::str { return self.ToString(); });
    }

    {
        using Class = ::renderer::TextureManager;
        constexpr auto* ClassName = "TextureManager";  // NOLINT
        py::class_<Class, Class::ptr>(m, ClassName)
            .def(py::init<>())
            .def("LoadTexture", &Class::LoadTexture)
            .def("CacheTexture", &Class::CacheTexture)
            .def("GetTexture", &Class::GetTexture)
            .def("DeleteTexture", &Class::DeleteTexture)
            .def("GetTextureByIndex", &Class::GetTextureByIndex)
            .def("GetNumTextures", &Class::GetNumTextures)
            .def("__getitem__",
                 [](Class& self, const std::string& tex_name) -> Texture::ptr {
                     return self.GetTexture(tex_name);
                 })
            .def("__getitem__",
                 [](Class& self, uint32_t tex_idx) -> Texture::ptr {
                     return self.GetTextureByIndex(tex_idx);
                 })
            .def("__len__",
                 [](const Class& self) -> uint32_t {
                     return self.GetNumTextures();
                 })
            .def("__repr__",
                 [](const Class& self) -> py::str { return self.ToString(); });
    }
}

}  // namespace renderer
