#include <pybind11/pybind11.h>

#include <renderer/input/buttons.hpp>

namespace py = pybind11;

namespace renderer {

// NOLINTNEXTLINE
auto bindings_buttons(py::module& m) -> void {
    {
        using Enum = ::renderer::button_action::ButtonAction;
        py::enum_<Enum>(m, "ButtonAction")
            .value("BUTTON_RELEASED", Enum::BUTTON_RELEASED)
            .value("BUTTON_PRESSED", Enum::BUTTON_PRESSED);
    }

    {
        using Enum = ::renderer::mouse::Mouse;
        py::enum_<Enum>(m, "Mouse")
            .value("BUTTON_LEFT", Enum::BUTTON_LEFT)
            .value("BUTTON_RIGHT", Enum::BUTTON_RIGHT)
            .value("BUTTON_MIDDLE", Enum::BUTTON_MIDDLE);
    }
}

}  // namespace renderer
