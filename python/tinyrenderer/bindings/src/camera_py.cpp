
#include <camera_py.h>

namespace py = pybind11;

namespace engine
{
    void bindings_cameraBase( py::module& m )
    {
        /* Expose required enumerators */
        py::enum_< engine::eCameraType >( m, "CameraType", py::arithmetic() )
            .value( "BASE", engine::eCameraType::BASE )
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
            .def_readwrite( "zFar", &CCameraProjData::zFar );

        /* Expose Base-Camera class */
        py::class_< CICamera >( m, "ICamera" )
            .def_property( "position", 
                []( const CICamera* self ) -> py::array_t<float32>
                    {
                        return tinymath::vector_to_nparray<float32,3>( self->position() );
                    },
                []( CICamera* self, const py::array_t<float32>& pos )
                    {
                        self->setPosition( tinymath::nparray_to_vector<float32,3>( pos ) );
                    } )
            .def_property( "targetPoint",
                []( const CICamera* self ) -> py::array_t<float32>
                    {
                        return tinymath::vector_to_nparray<float32,3>( self->targetPoint() );
                    },
                []( CICamera* self, const py::array_t<float32>& tpoint )
                    {
                        self->setTargetPoint( tinymath::nparray_to_vector<float32,3>( tpoint ) );
                    } )
            .def_property( "active",
                []( const CICamera* self ) -> bool
                    {
                        return self->active();
                    },
                []( CICamera* self, bool active )
                    {
                        self->setActiveMode( active ); 
                    } )
            .def_property( "projData", &CICamera::projData, &CICamera::setProjectionData )
            .def( "update", &CICamera::update )
            .def( "resize", &CICamera::resize )
            .def( "name", &CICamera::name )
            .def( "type", &CICamera::type )
            .def( "upAxis", &CICamera::upAxis )
            .def( "targetDir", []( const CICamera* self ) -> py::array_t<float32>
                {
                    return tinymath::vector_to_nparray<float32,3>( self->targetDir() );
                } )
            .def( "matView", []( const CICamera* self ) -> py::array_t<float32>
                {
                    return tinymath::matrix_to_nparray<float32,4>( self->matView() );
                } )
            .def( "matProj", []( const CICamera* self ) -> py::array_t<float32>
                {
                    return tinymath::matrix_to_nparray<float32,4>( self->matProj() );
                } )
            .def( "front", []( const CICamera* self ) -> py::array_t<float32>
                {
                    return tinymath::vector_to_nparray<float32,3>( self->front() );
                } )
            .def( "up", []( const CICamera* self ) -> py::array_t<float32>
                {
                    return tinymath::vector_to_nparray<float32,3>( self->up() );
                } )
            .def( "right", []( const CICamera* self ) -> py::array_t<float32>
                {
                    return tinymath::vector_to_nparray<float32,3>( self->right() );
                } )
            .def( "__repr__", []( const CICamera* self )
                {
                    auto _strrep = std::string( "Camera(\n" );
                    _strrep += "cpp-address     : " + tinyutils::PointerToHexAddress( self ) + "\n";
                    _strrep += self->toString();
                    return _strrep;
                } );
    }

    void bindings_cameraTypes( py::module& m )
    {
        /* Expose  Fixed-camera */
        py::class_< CFixedCamera, CICamera >( m, "FixedCamera" )
            .def( py::init( []( const std::string& name,
                                const py::array_t<float32>& position,
                                const py::array_t<float32>& targetPoint,
                                const eAxis& upAxis,
                                const CCameraProjData& projData )
                {
                    return std::make_unique<CFixedCamera>( name,
                                                           tinymath::nparray_to_vector<float32,3>( position ),
                                                           tinymath::nparray_to_vector<float32,3>( targetPoint ),
                                                           upAxis,
                                                           projData );
                } ) )
            .def( "setCameraTransform", []( CFixedCamera* self, const py::array_t<float32,4>& transform )
                {
                    self->setCameraTransform( tinymath::nparray_to_matrix<float32,4>( transform ) );
                } );

        /* Expose Fps-camera */
        py::class_< CFpsCamera, CICamera >( m, "FpsCamera" )
            .def( py::init( []( const std::string& name,
                                const py::array_t<float32>& position,
                                const py::array_t<float32>& targetPoint,
                                const eAxis& upAxis,
                                const CCameraProjData& projData,
                                float sensitivity,
                                float camSpeed,
                                float camMaxDelta )
                {
                    return std::make_unique<CFpsCamera>( name,
                                                         tinymath::nparray_to_vector<float32,3>( position ),
                                                         tinymath::nparray_to_vector<float32,3>( targetPoint ),
                                                         upAxis,
                                                         projData,
                                                         sensitivity,
                                                         camSpeed,
                                                         camMaxDelta );
                } ) )
            .def_property( "sensitivity", &CFpsCamera::sensitivity, &CFpsCamera::setSensitivity )
            .def_property( "camSpeed", &CFpsCamera::camSpeed, &CFpsCamera::setCamSpeed )
            .def_property( "camMaxDelta", &CFpsCamera::camMaxDelta, &CFpsCamera::setCamMaxDelta )
            .def( "camSpeedFront", &CFpsCamera::camSpeedFront )
            .def( "camSpeedRight", &CFpsCamera::camSpeedRight )
            .def( "roll", &CFpsCamera::roll )
            .def( "pitch", &CFpsCamera::pitch )
            .def( "yaw", &CFpsCamera::yaw );

        /* Expose Orbit-camera */
        py::class_< COrbitCamera, CICamera >( m, "OrbitCamera" )
            .def( py::init( []( const std::string& name,
                                const py::array_t<float32>& position,
                                const py::array_t<float32>& targetPoint,
                                const eAxis& upAxis,
                                const CCameraProjData& projData,
                                float viewportWidth,
                                float viewportHeight )
                {
                    return std::make_unique<COrbitCamera>( name,
                                                           tinymath::nparray_to_vector<float32,3>( position ),
                                                           tinymath::nparray_to_vector<float32,3>( targetPoint ),
                                                           upAxis,
                                                           projData,
                                                           viewportWidth,
                                                           viewportHeight,
                                                           0.005f,
                                                           1.000f );
                } ) )
            .def_property( "moveSensitivity", &COrbitCamera::moveSensitivity, &COrbitCamera::setMoveSensitivity )
            .def_property( "zoomSensitivity", &COrbitCamera::zoomSensitivity, &COrbitCamera::setZoomSensitivity );
    }
}