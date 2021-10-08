
#include <scene_py.h>

namespace py = pybind11;

namespace engine
{
    void bindings_scene( py::module& m )
    {
        // Expose CScene class (main container for rendering)
        py::class_< CScene >( m, "Scene" )
            .def( py::init<>() )
            .def( "AddCamera", &CScene::AddCamera, py::arg( "camera" ),
                  py::return_value_policy::reference,
                  py::keep_alive<1, 2>() )
            .def( "AddLight", &CScene::AddLight, py::arg( "light" ),
                  py::return_value_policy::reference,
                  py::keep_alive<1, 2>() )
            .def( "AddRenderable", &CScene::AddRenderable, py::arg( "renderable" ),
                  py::return_value_policy::reference,
                  py::keep_alive<1, 2>() )
            .def( "ChangeCurrentCamera", &CScene::ChangeCurrentCamera, py::arg( "name" ) )
            .def( "ChangeCurrentLight", &CScene::ChangeCurrentLight, py::arg( "name" ) )
            .def( "Update", &CScene::Update )
            .def( "Resize", &CScene::Resize )
            .def( "RemoveAllRenderables", &CScene::RemoveAllRenderables )
            .def( "GetCurrentCamera", []( CScene* self ) -> CICamera*
                {
                    return self->GetCurrentCamera();
                },
                py::return_value_policy::reference )
            .def( "GetCurrentLight", []( CScene* self ) -> CILight*
                {
                    return self->GetCurrentLight();
                },
                py::return_value_policy::reference )
            .def( "GetCameraByName", []( CScene* self, const std::string& name ) -> CICamera*
                {
                    return self->GetCameraByName( name );
                },
                py::arg( "name" ), py::return_value_policy::reference )
            .def( "GetLightByName", []( CScene* self, const std::string& name ) -> CILight*
                {
                    return self->GetLightByName( name );
                },
                py::arg( "name" ), py::return_value_policy::reference )
            .def( "GetRenderableByName", []( CScene* self, const std::string& name ) -> CIRenderable*
                {
                    return self->GetRenderableByName( name );
                },
                py::arg( "name" ), py::return_value_policy::reference )
            .def( "GetNumCameras", &CScene::GetNumCameras )
            .def( "GetNumLights", &CScene::GetNumLights )
            .def( "GetNumRenderables", &CScene::GetNumRenderables )
            .def( "HasCurrentFog", &CScene::HasCurrentFog )
            .def( "HasCurrentSkybox", &CScene::HasCurrentSkybox )
            .def( "HasCurrentCamera", &CScene::HasCurrentCamera )
            .def( "HasCurrentLight", &CScene::HasCurrentLight )
            .def( "HasCameraNamed", &CScene::HasCameraNamed, py::arg( "name" ) )
            .def( "HasLightNamed", &CScene::HasLightNamed, py::arg( "name" ) )
            .def( "HasRenderableNamed", &CScene::HasRenderableNamed, py::arg( "name" ) );
    }
}
