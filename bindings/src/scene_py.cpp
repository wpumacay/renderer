
#include <scene_py.h>

namespace py = pybind11;

namespace engine
{
    void bindings_scene( py::module& m )
    {
        // Expose CScene class (main container for rendering)
        py::class_< CScene >( m, "Scene" )
            .def( py::init<>() )
            .def( "addCamera", &CScene::addCamera, py::arg( "camera" ),
                  py::return_value_policy::reference, 
                  py::keep_alive<1, 2>() )
            .def( "addLight", &CScene::addLight, py::arg( "light" ),
                  py::return_value_policy::reference,
                  py::keep_alive<1, 2>() )
            .def( "addRenderable", &CScene::addRenderable, py::arg( "renderable" ),
                  py::return_value_policy::reference,
                  py::keep_alive<1, 2>() )
            .def( "update", &CScene::update )
            .def( "resize", &CScene::resize );
    }
}