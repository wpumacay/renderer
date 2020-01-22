
#include <application_py.h>

namespace py = pybind11;

namespace engine
{

    void bindings_application( py::module& m )
    {
        /* Expose CApplication class @wip */
        py::class_< CApplication >( m, "Application" )
            .def( py::init<>() )
            .def( py::init< const CWindowProps& >() )
            .def( "begin", &CApplication::begin )
            .def( "render", &CApplication::render )
            .def( "update", &CApplication::update )
            .def( "end", &CApplication::end )
            .def( "active", &CApplication::active )
            .def( "addKeyboardCallback", &CApplication::addKeyboardCallback )
            .def( "addMouseButtonCallback", &CApplication::addMouseButtonCallback )
            .def( "addMouseMoveCallback", &CApplication::addMouseMoveCallback )
            .def( "addScrollCallback", &CApplication::addScrollCallback )
            .def( "addResizeCallback", &CApplication::addResizeCallback )
            .def( "window", &CApplication::window );
    }

}