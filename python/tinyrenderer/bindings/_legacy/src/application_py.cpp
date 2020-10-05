
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
            .def( "setScene", &CApplication::setScene, py::arg( "scene" ),
                  py::return_value_policy::reference,
                  py::keep_alive<1, 2>() )
            .def( "setOffScreenRendering", &CApplication::setOffscreenRendering )
            .def( "begin", &CApplication::begin )
            .def( "render", &CApplication::render )
            .def( "update", &CApplication::update )
            .def( "end", &CApplication::end )
            .def( "active", &CApplication::active )
            .def( "renderOptions", []( CApplication* self ) -> CRenderOptions*
                {
                    return &self->renderOptions();
                }, py::return_value_policy::reference, py::keep_alive< 1, 0 >() )
            .def( "renderTarget", &CApplication::renderTarget, py::return_value_policy::reference )
            .def( "addKeyboardCallback", &CApplication::addKeyboardCallback )
            .def( "addMouseButtonCallback", &CApplication::addMouseButtonCallback )
            .def( "addMouseMoveCallback", &CApplication::addMouseMoveCallback )
            .def( "addScrollCallback", &CApplication::addScrollCallback )
            .def( "addResizeCallback", &CApplication::addResizeCallback )
            .def( "scene", &CApplication::scene, py::return_value_policy::reference ) // @firsttodo: try <1,0>
            .def( "window", &CApplication::window, py::return_value_policy::reference );
    }

}