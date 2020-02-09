
#include <math_py.h>

namespace py = pybind11;

//// Usage of py::array_t<float32> is chosen over py::buffer to let the user send the type of data
//// that he's more comfortable (lists, ndarray, tm.Vector). This is slower, as it requires casting
//// (py::array_t<other_type> -> py::array_t<float32>), and a convertion to the internal vector types
//// (CVecX, CMatY). We might run into the requirement of having to read|write some elements of some
//// exposed objects, so it's better to avoid doing unnecessary read|write operations to avoid this
//// extra overhead.

namespace engine
{
    void bindings_mathHelpers( py::module& m )
    {
        m.def( "perspective", &engine::perspective );
        m.def( "ortho", &engine::ortho );
        m.def( "lookAt", []( const py::array_t<float32>& arr_position,
                             const py::array_t<float32>& arr_target,
                             const py::array_t<float32>& arr_worldUp )
            {
                return engine::lookAt( tinymath::nparray_to_vector<float32,3>( arr_position ),
                                       tinymath::nparray_to_vector<float32,3>( arr_target ),
                                       tinymath::nparray_to_vector<float32,3>( arr_worldUp ) );
            } );
        m.def( "scale", []( const py::array_t<float32>& arr_scale )
            {
                return engine::scale( tinymath::nparray_to_vector<float32,3>( arr_scale ) );
            } );
        m.def( "rotation", []( const py::array_t<float32>& arr_mat3 )
            {
                return engine::rotation( tinymath::nparray_to_matrix<float32,3>( arr_mat3 ) );
            } );
        m.def( "translation", []( const py::array_t<float32>& arr_translation )
            {
                return engine::translation( tinymath::nparray_to_vector<float32,3>( arr_translation ) );
            } );

        m.def( "rotationX", &engine::rotationX );
        m.def( "rotationY", &engine::rotationY );
        m.def( "rotationZ", &engine::rotationZ );

        py::class_< CLine >( m, "Line" )
            .def( py::init<>() )
            .def( py::init( []( const py::array_t<float32>& arr_start,
                                const py::array_t<float32>& arr_end )
                {
                    return std::make_unique<CLine>( tinymath::nparray_to_vector<float32,3>( arr_start ),
                                                    tinymath::nparray_to_vector<float32,3>( arr_end ) );
                } ) )
            .def_readwrite( "start", &CLine::start )
            .def_readwrite( "end", &CLine::end )
            .def( "__repr__", []( const CLine* self )
                {
                    auto _strrep = std::string( "Line(\n" );
                    _strrep += "start   : " + engine::toString( self->start ) + "\n";
                    _strrep += "end     : " + engine::toString( self->end ) + ")";
                    return _strrep;
                } );

        m.def( "computeFrameAxes", []( const py::array_t<float32>& arr_axis1,
                                       const py::array_t<float32>& arr_worldUp ) -> std::pair<CVec3, CVec3>
                {
                    CVec3 axis2, axis3;
                    engine::computeFrameAxes( tinymath::nparray_to_vector<float32,3>( arr_axis1 ),
                                              axis2, axis3,
                                              tinymath::nparray_to_vector<float32,3>( arr_worldUp ) );
                    return std::make_pair( axis2, axis3 );
                } );

        py::class_< CPlane >( m, "Plane" )
            .def( py::init<>() )
            .def_readwrite( "normal", &CPlane::normal )
            .def_readwrite( "position", &CPlane::position )
            .def( "__repr__", []( const CPlane* self )
                {
                    auto _strrep = std::string( "Plane(\n" );
                    _strrep += "normal  : " + engine::toString( self->normal ) + "\n";
                    _strrep += "position: " + engine::toString( self->position ) + ")";
                    return _strrep;
                } );

        m.def( "signedDistToPlane", []( const py::array_t<float32>& arr_point, const CPlane* plane ) -> float32
            {
                return engine::signedDistToPlane( tinymath::nparray_to_vector<float32,3>( arr_point ), *plane );
            } );
        m.def( "distToPlane", []( const py::array_t<float32>& arr_point, const CPlane* plane ) -> float32
            {
                return engine::distToPlane( tinymath::nparray_to_vector<float32,3>( arr_point ), *plane );
            } );
        m.def( "projInPlane", []( const py::array_t<float32>& arr_point, const CPlane* plane ) -> py::array_t<float32>
            {
                return tinymath::vector_to_nparray<float32,3>( engine::projInPlane( tinymath::nparray_to_vector<float32,3>( arr_point ), *plane ) );
            } );

        py::class_< CBoundingBox >( m, "BoundingBox" )
            .def( py::init<>() )
            .def_readwrite( "size", &CBoundingBox::size )
            .def_readwrite( "transform", &CBoundingBox::worldTransform )
            .def( "__repr__", []( const CBoundingBox* self )
                {
                    auto _strrep = std::string( "BoundingBox(\n" );
                    _strrep += "size : " + engine::toString( self->size ) + "\n";
                    _strrep += "transform:\n" + engine::toString( self->worldTransform ) + ")";
                    return _strrep;
                } );

        m.def( "computeBoxCorners", &engine::computeBoxCorners );

        py::class_< CBoundingSphere >( m, "BoundingSphere" )
            .def( py::init<>() )
            .def_readwrite( "radius", &CBoundingSphere::radius )
            .def_readwrite( "position", &CBoundingSphere::worldPosition )
            .def( "__repr__", []( const CBoundingSphere* self )
                {
                    auto _strrep = std::string( "BoundingSphere(\n" );
                    _strrep += "radius  : " + std::to_string( self->radius ) + "\n";
                    _strrep += "position: " + engine::toString( self->worldPosition ) + ")";
                    return _strrep;
                } );

        py::class_< CFrustum >( m, "Frustum" )
            .def( py::init( []( const py::array_t<float32>& arr_viewProjMatrix )
                {
                    return std::make_unique<CFrustum>( tinymath::nparray_to_matrix<float32,4>( arr_viewProjMatrix ) );
                } ) )
            .def_property_readonly( "corners", []( const CFrustum* self ) -> std::array<CVec3, 8>
                {
                    return self->corners;
                } )
            .def_property_readonly( "planes", []( const CFrustum* self ) -> std::array<CPlane, 6>
                {
                    return self->planes;
                } )
            .def( "__repr__", []( const CFrustum* self )
                {
                    auto _strrep = std::string( "Frustum(\n" );
                    _strrep += "corners:\n";
                    for ( size_t i = 0; i < self->corners.size(); i++ )
                        _strrep += "\tposition: " + engine::toString( self->corners[i] ) + "\n";
                    _strrep += "planes:\n";
                    for ( size_t i = 0; i < self->planes.size(); i++ )
                        _strrep += "\tnormal: " + engine::toString( self->planes[i].normal ) + " - position: " + engine::toString( self->planes[i].position ) + "\n";
                    _strrep += ")";
                    return _strrep;
                } );

        m.def( "certainlyOutsideFrustum", (bool (*)(const CFrustum&, const CBoundingBox&)) &engine::certainlyOutsideFrustum );
        m.def( "certainlyOutsideFrustum", (bool (*)(const CFrustum&, const CBoundingSphere&)) &engine::certainlyOutsideFrustum );

        m.def( "computeMinMaxVertexToPlane", []( const CPlane* plane, const CBoundingBox* bbox ) -> std::pair<CVec3,CVec3>
            {
                CVec3 minVertex, maxVertex;
                engine::computeMinMaxVertexToPlane( *plane, *bbox, minVertex, maxVertex );
                return std::make_pair( minVertex, maxVertex );
            } );
    }
}