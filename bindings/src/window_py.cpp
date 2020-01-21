
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
            .def_readwrite( "resizable", &CWindowProps::resizable )
            .def_readwrite( "gl_api_major", &CWindowProps::gl_api_version_major )
            .def_readwrite( "gl_api_minor", &CWindowProps::gl_api_version_minor )
            .def_readwrite( "width", &CWindowProps::width );

        // Expose COpenGLWindow Class
        py::class_< COpenGLWindow >( m, "OpenGlWindow" )
            .def( py::init< const CWindowProps& >() )
            .def( "begin", &COpenGLWindow::begin )
            .def( "end", &COpenGLWindow::end )
            .def( "active", &COpenGLWindow::active )
            .def( "requestClose", &COpenGLWindow::requestClose )
            .def( "test_callback", []( COpenGLWindow& self, std::function<void(int,int)> f )
            {
                std::cout << "callback - test" << std::endl;
                f(10,20);
            } )
            .def( "registerCallbackKey", &COpenGLWindow::registerKeyCallback )
            .def_property_readonly( "width", &COpenGLWindow::width )
            .def_property_readonly( "height", &COpenGLWindow::height )
            .def_property_readonly( "aspect", &COpenGLWindow::aspect )
            .def_property_readonly( "title", &COpenGLWindow::title );
    }
}

