
#include <window_py.h>

namespace py = pybind11;

namespace engine
{
    void bindings_windowBase( py::module& m )
    {
        // Expose CWindowProps Struct
        py::class_< CWindowProps >( m, "WindowProps" )
            .def( py::init<>() )
            .def_readwrite( "width", &CWindowProps::width )
            .def_readwrite( "height", &CWindowProps::height )
            .def_readwrite( "title", &CWindowProps::title )
            //// .def_readwrite( "clearColor", &CWindowProps::clearColor ) // this works, but we implement the part below for further compatibility
            .def_property( "clearColor",
                []( const CWindowProps& self ) -> py::array_t<tinymath::tfloat>
                    {
                        return tinymath::vector_to_nparray<tinymath::tfloat,4>( self.clearColor );
                    },
                []( CWindowProps& self, const py::array_t<tinymath::tfloat>& color ) -> void
                    {
                        self.clearColor = tinymath::nparray_to_vector<tinymath::tfloat,4>( color );
                    } )
            .def_readwrite( "resizable", &CWindowProps::resizable )
            .def_readwrite( "gl_api_major", &CWindowProps::gl_api_version_major )
            .def_readwrite( "gl_api_minor", &CWindowProps::gl_api_version_minor )
            .def_readwrite( "width", &CWindowProps::width );

        // Expose CIWindow Class
        py::class_< CIWindow >( m, "IWindow" )
            .def( "begin", &CIWindow::begin )
            .def( "end", &CIWindow::end )
            .def( "active", &CIWindow::active )
            .def( "requestClose", &CIWindow::requestClose )
            .def( "registerKeyCallback", &CIWindow::registerKeyCallback )
            .def( "registerMouseCallback", &CIWindow::registerMouseCallback )
            .def( "registerMouseMoveCallback", &CIWindow::registerMouseMoveCallback )
            .def( "registerScrollCallback", &CIWindow::registerScrollCallback )
            .def_property_readonly( "width", &CIWindow::width )
            .def_property_readonly( "height", &CIWindow::height )
            .def_property_readonly( "aspect", &CIWindow::aspect )
            .def_property_readonly( "title", &CIWindow::title );
    }

#ifndef ENGINE_HEADLESS_EGL
    void bindings_windowGLFW( py::module& m )
    {
        py::class_< CWindowGLFW, CIWindow >( m, "WindowGLFW" )
            .def( py::init< const CWindowProps& >() );
    }
#else
    void bindings_windowEGL( py::module& m )
    {
        py::class_< CWindowEGL, CIWindow >( m, "WindowEGL" )
            .def( py::init< const CWindowProps& >() );
    }
#endif /* ENGINE_HEADLESS_EGL */
}

