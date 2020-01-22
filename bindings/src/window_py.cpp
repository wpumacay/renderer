
#include <window_py.h>
// pytbind11 - functional (for callbacks support)
#include <pybind11/functional.h>

namespace py = pybind11;

namespace engine
{
    void bindings_window( py::module& m )
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

        // Expose COpenGLWindow Class
        py::class_< COpenGLWindow >( m, "OpenGLWindow" )
            .def( py::init< const CWindowProps& >() )
            .def( "begin", &COpenGLWindow::begin )
            .def( "end", &COpenGLWindow::end )
            .def( "active", &COpenGLWindow::active )
            .def( "requestClose", &COpenGLWindow::requestClose )
            .def( "registerKeyCallback", &COpenGLWindow::registerKeyCallback )
            .def( "registerMouseCallback", &COpenGLWindow::registerMouseCallback )
            .def( "registerMouseMoveCallback", &COpenGLWindow::registerMouseMoveCallback )
            .def( "registerScrollCallback", &COpenGLWindow::registerScrollCallback )
            .def_property_readonly( "width", &COpenGLWindow::width )
            .def_property_readonly( "height", &COpenGLWindow::height )
            .def_property_readonly( "aspect", &COpenGLWindow::aspect )
            .def_property_readonly( "title", &COpenGLWindow::title );
    }
}

