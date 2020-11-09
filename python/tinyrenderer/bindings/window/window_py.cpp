
#include <common_py.h>
#include <window/CWindowGLFW.h>
#include <window/CWindowEGL.h>

namespace py = pybind11;

namespace engine
{
    void bindings_window( py::module& m )
    {
        // Expose CWindowProps Struct
        py::class_<CWindowProps>( m, "WindowProps" )
            .def( py::init<>() )
            .def_readwrite( "width", &CWindowProps::width )
            .def_readwrite( "height", &CWindowProps::height )
            .def_readwrite( "title", &CWindowProps::title )
            .def_property( "clearColor",
                []( const CWindowProps& self ) -> py::array_t<engine::float32>
                    {
                        return tinymath::vector_to_nparray<engine::float32,4>( self.clearColor );
                    },
                []( CWindowProps& self, const py::array_t<engine::float32>& color )
                    {
                        self.clearColor = tinymath::nparray_to_vector<engine::float32,4>( color );
                    } )
            .def_readwrite( "resizable", &CWindowProps::resizable )
            .def_readwrite( "gl_api_major", &CWindowProps::gl_api_version_major )
            .def_readwrite( "gl_api_minor", &CWindowProps::gl_api_version_minor )
            .def_readwrite( "width", &CWindowProps::width );

        // Expose CIWindow Class
        py::class_<CIWindow>( m, "IWindow" )
            .def( "Begin", &CIWindow::Begin )
            .def( "End", &CIWindow::End )
            .def( "Active", &CIWindow::Active )
            .def( "RequestClose", &CIWindow::RequestClose )
            .def( "RegisterKeyCallback", &CIWindow::RegisterKeyCallback )
            .def( "RegisterMouseCallback", &CIWindow::RegisterMouseCallback )
            .def( "RegisterMouseMoveCallback", &CIWindow::RegisterMouseMoveCallback )
            .def( "RegisterScrollCallback", &CIWindow::RegisterScrollCallback )
            .def_property_readonly( "width", &CIWindow::width )
            .def_property_readonly( "height", &CIWindow::height )
            .def_property_readonly( "aspect", &CIWindow::aspect )
            .def_property_readonly( "title", &CIWindow::title )
            .def( "properties", []( CIWindow& self ) -> CWindowProps&
                {
                    return self.properties();
                } );

    // @todo: Should move backend-implementation to internal adapter instaed of sub-classing
    #ifdef ENGINE_GLFW_ENABLED
        py::class_<CWindowGLFW, CIWindow>( m, "WindowGLFW" )
            .def( py::init< const CWindowProps& >() );
    #endif /* ENGINE_GLFW_ENABLED */

    // @todo: Should move backend-implementation to internal adapter instaed of sub-classing
    #ifdef ENGINE_EGL_ENABLED
        py::class_<CWindowEGL, CIWindow>( m, "WindowEGL" )
            .def( py::init< const CWindowProps& >() );
    #endif /* ENGINE_EGL_ENABLED */
    }
}