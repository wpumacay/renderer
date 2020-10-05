
#include <camera_py.h>

namespace py = pybind11;

namespace engine
{
    void bindings_cameraBase( py::module& m )
    {
        /* Expose required enumerators */
        py::enum_< engine::eCameraType >( m, "CameraType", py::arithmetic() )
            .value( "FIXED", engine::eCameraType::FIXED )
            .value( "FPS", engine::eCameraType::FPS )
            .value( "ORBIT", engine::eCameraType::ORBIT );

        py::enum_< engine::eCameraProjection >( m, "CameraProjection", py::arithmetic() )
            .value( "PERSPECTIVE", engine::eCameraProjection::PERSPECTIVE )
            .value( "ORTHOGRAPHIC", engine::eCameraProjection::ORTHOGRAPHIC );

        /* Expose build|config struct for cameras */
        py::class_< CCameraProjData >( m, "CameraProjData" )
            .def( py::init<>() )
            .def_readwrite( "projection", &CCameraProjData::projection )
            .def_readwrite( "fov", &CCameraProjData::fov )
            .def_readwrite( "aspect", &CCameraProjData::aspect )
            .def_readwrite( "width", &CCameraProjData::width )
            .def_readwrite( "height", &CCameraProjData::height )
            .def_readwrite( "zNear", &CCameraProjData::zNear )
            .def_readwrite( "zFar", &CCameraProjData::zFar )
            .def_readwrite( "viewportWidth", &CCameraProjData::viewportWidth )
            .def_readwrite( "viewportHeight", &CCameraProjData::viewportHeight );

        /* Expose Base-Camera class */
        py::class_< CICamera >( m, "ICamera" )
            .def_property( "position", 
                []( const CICamera* self ) -> py::array_t<float32>
                    {
                        return tinymath::vector_to_nparray<float32,3>( self->position() );
                    },
                []( CICamera* self, const py::array_t<float32>& pos )
                    {
                        self->SetPosition( tinymath::nparray_to_vector<float32,3>( pos ) );
                    } )
            .def_property( "target_point",
                []( const CICamera* self ) -> py::array_t<float32>
                    {
                        return tinymath::vector_to_nparray<float32,3>( self->target_point() );
                    },
                []( CICamera* self, const py::array_t<float32>& tpoint )
                    {
                        self->SetTargetPoint( tinymath::nparray_to_vector<float32,3>( tpoint ) );
                    } )
            .def_property( "active",
                []( const CICamera* self ) -> bool
                    {
                        return self->active();
                    },
                []( CICamera* self, bool active )
                    {
                        self->SetActiveMode( active ); 
                    } )
            .def_property( "proj_data",
                []( const CICamera* self ) -> const CCameraProjData&
                    {
                        return self->proj_data();
                    },
                []( CICamera* self, const CCameraProjData& proj_data ) -> void
                    {
                        self->SetProjectionData( proj_data );
                    } )
            .def( "update", &CICamera::Update )
            .def( "resize", &CICamera::Resize )
            .def_property_readonly( "name", &CICamera::name )
            .def_property_readonly( "type", &CICamera::type )
            .def_property_readonly( "up_axis", &CICamera::up_axis )
            .def_property_readonly( "target_direction", []( const CICamera* self ) -> py::array_t<float32>
                {
                    return tinymath::vector_to_nparray<float32,3>( self->target_direction() );
                } )
            .def_property_readonly( "front", []( const CICamera* self ) -> py::array_t<float32>
                {
                    return tinymath::vector_to_nparray<float32,3>( self->front() );
                } )
            .def_property_readonly( "up", []( const CICamera* self ) -> py::array_t<float32>
                {
                    return tinymath::vector_to_nparray<float32,3>( self->up() );
                } )
            .def_property_readonly( "right", []( const CICamera* self ) -> py::array_t<float32>
                {
                    return tinymath::vector_to_nparray<float32,3>( self->right() );
                } )
            .def( "mat_view", []( const CICamera* self ) -> py::array_t<float32>
                {
                    return tinymath::matrix_to_nparray<float32,4>( self->mat_view() );
                } )
            .def( "mat_proj", []( const CICamera* self ) -> py::array_t<float32>
                {
                    return tinymath::matrix_to_nparray<float32,4>( self->mat_proj() );
                } )
            .def( "__repr__", []( const CICamera* self )
                {
                    auto strrep = std::string( "Camera(\n" );
                    strrep += "cpp-address     : " + tinyutils::PointerToHexAddress( self ) + "\n";
                    strrep += self->ToString();
                    strrep += ")";
                    return strrep;
                } );
    }

    void bindings_cameraTypes( py::module& m )
    {
        /* Expose  Fixed-camera */
        py::class_< CFixedCamera, CICamera >( m, "FixedCamera" )
            .def( py::init( []( const std::string& name,
                                const py::array_t<float32>& position,
                                const py::array_t<float32>& target_point,
                                const eAxis& up_axis,
                                const CCameraProjData& proj_data )
                {
                    return std::make_unique<CFixedCamera>( name,
                                                           tinymath::nparray_to_vector<float32,3>( position ),
                                                           tinymath::nparray_to_vector<float32,3>( target_point ),
                                                           up_axis,
                                                           proj_data );
                } ), py::arg( "name" ), py::arg( "position" ), py::arg( "target_point" ), py::arg( "up_axis" ), py::arg( "proj_data" ) )
            .def( "SetCameraTransform", []( CFixedCamera* self, const py::array_t<float32,4>& transform )
                {
                    self->SetCameraTransform( tinymath::nparray_to_matrix<float32,4>( transform ) );
                } );

        /* Expose Orbit-camera */
        py::class_< COrbitCamera, CICamera >( m, "OrbitCamera" )
            .def( py::init( []( const std::string& name,
                                const py::array_t<float32>& position,
                                const py::array_t<float32>& target_point,
                                const eAxis& up_axis,
                                const CCameraProjData& proj_data,
                                float move_sensitivity,
                                float zoom_sensitivity )
                {
                    return std::make_unique<COrbitCamera>( name,
                                                           tinymath::nparray_to_vector<float32,3>( position ),
                                                           tinymath::nparray_to_vector<float32,3>( target_point ),
                                                           up_axis,
                                                           proj_data,
                                                           move_sensitivity,
                                                           zoom_sensitivity );
                } ), py::arg( "name" ), py::arg( "position" ), py::arg( "target_point" ), py::arg( "up_axis" ),
                py::arg( "proj_data" ), py::arg( "move_sensitivity" ) = 0.005f, py::arg( "zoom_sensitivity" ) = 1.000f )
            .def_property( "move_sensitivity", &COrbitCamera::move_sensitivity, &COrbitCamera::SetMoveSensitivity )
            .def_property( "zoom_sensitivity", &COrbitCamera::zoom_sensitivity, &COrbitCamera::SetZoomSensitivity );

        /* Expose Fps-camera */
        py::class_< CFpsCamera, CICamera >( m, "FpsCamera" )
            .def( py::init( []( const std::string& name,
                                const py::array_t<float32>& position,
                                const py::array_t<float32>& target_point,
                                const eAxis& up_axis,
                                const CCameraProjData& proj_data,
                                float sensitivity,
                                float speed,
                                float max_delta )
                {
                    return std::make_unique<CFpsCamera>( name,
                                                         tinymath::nparray_to_vector<float32,3>( position ),
                                                         tinymath::nparray_to_vector<float32,3>( target_point ),
                                                         up_axis,
                                                         proj_data,
                                                         sensitivity,
                                                         speed,
                                                         max_delta );
                } ), py::arg( "name" ), py::arg( "position" ), py::arg( "target_point" ), py::arg( "up_axis" ), 
                py::arg( "proj_data" ), py::arg( "sensitivity" ) = 0.25f, py::arg( "speed" ) = 250.0f, py::arg( "max_delta" ) = 10.0f )
            .def_property( "sensitivity", &CFpsCamera::sensitivity, &CFpsCamera::SetSensitivity )
            .def_property( "speed", &CFpsCamera::speed, &CFpsCamera::SetSpeed )
            .def_property( "max_delta", &CFpsCamera::max_delta, &CFpsCamera::SetMaxDelta )
            .def_property_readonly( "current_front_speed", &CFpsCamera::current_front_speed )
            .def_property_readonly( "current_right_speed", &CFpsCamera::current_right_speed )
            .def_property_readonly( "roll", &CFpsCamera::roll )
            .def_property_readonly( "pitch", &CFpsCamera::pitch )
            .def_property_readonly( "yaw", &CFpsCamera::yaw );
    }
}