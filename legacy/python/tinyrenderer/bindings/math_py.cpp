
#include <common_py.h>

namespace py = pybind11;

//// Usage of py::array_t<float32> is chosen over py::buffer to let the user send the type of data
//// that he's more comfortable (lists, ndarray, tm.Vector). This is slower, as it requires casting
//// (py::array_t<other_type> -> py::array_t<float32>), and a convertion to the internal vector types
//// (CVecX, CMatY). We might run into the requirement of having to read|write some elements of some
//// exposed objects, so it's better to avoid doing unnecessary read|write operations to avoid this
//// extra overhead.

namespace engine
{
    void bindings_math( py::module& m )
    {
        m.def( "Perspective", &engine::Perspective );
        m.def( "Ortho", &engine::Ortho );
        m.def( "LookAt", []( const py::array_t<float32>& arr_position,
                             const py::array_t<float32>& arr_target,
                             const py::array_t<float32>& arr_worldUp )
            {
                return engine::LookAt( tinymath::nparray_to_vector<float32,3>( arr_position ),
                                       tinymath::nparray_to_vector<float32,3>( arr_target ),
                                       tinymath::nparray_to_vector<float32,3>( arr_worldUp ) );
            } );
        m.def( "Scale", []( const py::array_t<float32>& arr_scale )
            {
                return engine::Scale( tinymath::nparray_to_vector<float32,3>( arr_scale ) );
            } );
        m.def( "Rotation", []( const py::array_t<float32>& arr_mat3 )
            {
                return engine::Rotation( tinymath::nparray_to_matrix<float32,3>( arr_mat3 ) );
            } );
        m.def( "Translation", []( const py::array_t<float32>& arr_translation )
            {
                return engine::Translation( tinymath::nparray_to_vector<float32,3>( arr_translation ) );
            } );

        m.def( "RotationX", &engine::RotationX );
        m.def( "RotationY", &engine::RotationY );
        m.def( "RotationZ", &engine::RotationZ );

        py::class_<CLine>( m, "Line" )
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
                    auto strrep = std::string( "Line(\n" );
                    strrep += "start   : " + engine::ToString( self->start ) + "\n";
                    strrep += "end     : " + engine::ToString( self->end ) + ")";
                    return strrep;
                } );

        m.def( "ComputeFrameAxes", []( const py::array_t<float32>& arr_axis1,
                                       const py::array_t<float32>& arr_worldUp ) -> std::pair<CVec3, CVec3>
                {
                    CVec3 axis2, axis3;
                    engine::ComputeFrameAxes( tinymath::nparray_to_vector<float32,3>( arr_axis1 ),
                                              axis2, axis3,
                                              tinymath::nparray_to_vector<float32,3>( arr_worldUp ) );
                    return std::make_pair( axis2, axis3 );
                } );

        py::class_<CPlane>( m, "Plane" )
            .def( py::init<>() )
            .def_readwrite( "normal", &CPlane::normal )
            .def_readwrite( "position", &CPlane::position )
            .def( "__repr__", []( const CPlane* self )
                {
                    auto strrep = std::string( "Plane(\n" );
                    strrep += "normal  : " + engine::ToString( self->normal ) + "\n";
                    strrep += "position: " + engine::ToString( self->position ) + ")";
                    return strrep;
                } );

        m.def( "SignedDistToPlane", []( const py::array_t<float32>& arr_point, const CPlane& plane ) -> float32
            {
                return engine::SignedDistToPlane( tinymath::nparray_to_vector<float32,3>( arr_point ), plane );
            } );
        m.def( "DistToPlane", []( const py::array_t<float32>& arr_point, const CPlane& plane ) -> float32
            {
                return engine::DistToPlane( tinymath::nparray_to_vector<float32,3>( arr_point ), plane );
            } );
        m.def( "ProjInPlane", []( const py::array_t<float32>& arr_point, const CPlane& plane ) -> py::array_t<float32>
            {
                return tinymath::vector_to_nparray<float32,3>(
                                    engine::ProjInPlane( tinymath::nparray_to_vector<float32,3>( arr_point ), plane ) );
            } );

        py::class_<CBoundingBox>( m, "BoundingBox" )
            .def( py::init<>() )
            .def_readwrite( "size", &CBoundingBox::size )
            .def_readwrite( "tf", &CBoundingBox::tf )
            .def( "__repr__", []( const CBoundingBox* self )
                {
                    auto strrep = std::string( "BoundingBox(\n" );
                    strrep += "size        : " + engine::ToString( self->size ) + "\n";
                    strrep += "transform   :\n" + engine::ToString( self->tf ) + ")";
                    return strrep;
                } );

        m.def( "ComputeBoxCorners", &engine::ComputeBoxCorners );

        py::class_<CBoundingSphere>( m, "BoundingSphere" )
            .def( py::init<>() )
            .def_readwrite( "radius", &CBoundingSphere::radius )
            .def_readwrite( "center", &CBoundingSphere::center )
            .def( "__repr__", []( const CBoundingSphere* self )
                {
                    auto strrep = std::string( "BoundingSphere(\n" );
                    strrep += "radius  : " + std::to_string( self->radius ) + "\n";
                    strrep += "center  : " + engine::ToString( self->center ) + ")";
                    return strrep;
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
                    auto strrep = std::string( "Frustum(\n" );
                    strrep += "corners:\n";
                    for ( size_t i = 0; i < self->corners.size(); i++ )
                        strrep += "\tposition: " + engine::ToString( self->corners[i] ) + "\n";
                    strrep += "planes:\n";
                    for ( size_t i = 0; i < self->planes.size(); i++ )
                        strrep += "\tnormal: " + engine::ToString( self->planes[i].normal ) + " - position: " + engine::ToString( self->planes[i].position ) + "\n";
                    strrep += ")";
                    return strrep;
                } );

        m.def( "CertainlyOutsideFrustum", (bool (*)(const CFrustum&, const CBoundingBox&)) &engine::CertainlyOutsideFrustum );
        m.def( "CertainlyOutsideFrustum", (bool (*)(const CFrustum&, const CBoundingSphere&)) &engine::CertainlyOutsideFrustum );

        m.def( "ComputeMinMaxVertexToPlane", []( const CPlane& plane, const CBoundingBox& bbox ) -> std::pair<CVec3,CVec3>
            {
                CVec3 minVertex, maxVertex;
                engine::ComputeMinMaxVertexToPlane( plane, bbox, minVertex, maxVertex );
                return std::make_pair( minVertex, maxVertex );
            } );
    }
}
