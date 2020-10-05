
#include <debug_py.h>

namespace py = pybind11;

//// @todo: requires testing each function (thoroughly)

namespace engine
{
    void bindings_debugDrawer( py::module& m )
    {
        // Expose CDebugDrawer system (pure static helpers)
        py::class_< CDebugDrawer >( m, "DebugDrawer" )
            .def_static( "Init", &CDebugDrawer::Init )
            .def_static( "Release", &CDebugDrawer::Release )
            .def_static( "SetEnabled", &CDebugDrawer::SetEnabled )
            .def_static( "Render", (void (*)( CICamera* )) &CDebugDrawer::Render )
            // @todo: enable when lights are exposed
            //// .def_static( "Render", (void ( CDebugDrawer::* )( CICamera*, CILight* )) &CDebugDrawer::Render )
            .def_static( "DrawLine",
                         []( const py::array_t<float32>& arr_start,
                             const py::array_t<float32>& arr_end,
                             const py::array_t<float32>& arr_color )
                            {
                                CDebugDrawer::DrawLine( tinymath::nparray_to_vector<float32,3>( arr_start ),
                                                        tinymath::nparray_to_vector<float32,3>( arr_end ),
                                                        tinymath::nparray_to_vector<float32,3>( arr_color ) );
                            } )
            .def_static( "DrawBox",
                         []( const py::array_t<float32>& arr_size,
                             const py::array_t<float32>& arr_transform,
                             const py::array_t<float32>& arr_color )
                            {
                                CDebugDrawer::DrawBox( tinymath::nparray_to_vector<float32,3>( arr_size ),
                                                       tinymath::nparray_to_matrix<float32,4>( arr_transform ),
                                                       tinymath::nparray_to_vector<float32,3>( arr_color ) );
                            } )
            .def_static( "DrawSphere",
                         []( float32 radius,
                             const py::array_t<float32>& arr_transform )
                            {
                                CDebugDrawer::DrawSphere( radius,
                                                          tinymath::nparray_to_matrix<float32,4>( arr_transform ) );
                            } )
            .def_static( "DrawSphere",
                         []( float32 radius, 
                             const py::array_t<float32>& arr_transform, 
                             const py::array_t<float32>& arr_color )
                            {
                                CDebugDrawer::DrawSphere( radius,
                                                          tinymath::nparray_to_matrix<float32,4>( arr_transform ),
                                                          tinymath::nparray_to_vector<float32,3>( arr_color ) );
                            } )
            .def_static( "DrawArrow",
                         []( const py::array_t<float32>& arr_start,
                             const py::array_t<float32>& arr_end,
                             const py::array_t<float32>& arr_color )
                            {
                                CDebugDrawer::DrawArrow( tinymath::nparray_to_vector<float32,3>( arr_start ),
                                                         tinymath::nparray_to_vector<float32,3>( arr_end ),
                                                         tinymath::nparray_to_vector<float32,3>( arr_color ) );
                            } )
            .def_static( "DrawClipVolume",
                         []( const py::array_t<float32>& arr_clipFrustum,
                             const py::array_t<float32>& arr_color )
                            {
                                CDebugDrawer::DrawClipVolume( tinymath::nparray_to_matrix<float32,4>( arr_clipFrustum ),
                                                              tinymath::nparray_to_vector<float32,3>( arr_color ) );
                            } )
            .def_static( "DrawTrailPoints", (void (*)( const std::vector<CVec3>&, const CVec3& )) &CDebugDrawer::DrawTrailPoints )
            .def_static( "DrawTrailPoints", []( const py::array_t<float32>& arr_points, const CVec3& color )
                {
                    auto bufferInfo = arr_points.request();
                    if ( bufferInfo.ndim != 2 )
                    {
                        throw std::runtime_error( "tinyrenderer::DrawTrailPoints >>> expected an np.ndarray \
                                                   of dim=2 (#points,#3), but got dim=" + std::to_string( bufferInfo.ndim ) + "." );
                    }

                    if ( bufferInfo.shape[1] != 3 )
                    {
                        throw std::runtime_error( "tinyrenderer::DrawTrailPoints >>> points size should be 3, \
                                                   but got " + std::to_string( bufferInfo.shape[1] ) + " instead." );
                    }

                    const float32* bufferData = (float32*) bufferInfo.ptr;
                    const bool columnMajor = bufferInfo.strides[0] < bufferInfo.strides[1];
                    std::vector< CVec3 > points;
                    for ( size_t i = 0; i < bufferInfo.shape[0]; i++ )
                    {
                        CVec3 point;
                        for ( size_t j = 0; j < bufferInfo.shape[1]; i++ )
                        {
                            if ( columnMajor ) // use this indexing as vectors are in the rows (#points,#3)
                                point[j] = bufferData[i + j * bufferInfo.shape[0]];
                            else
                                point[j] = bufferData[j + i * bufferInfo.shape[1]];
                        }
                        points.push_back( point );
                    }

                    CDebugDrawer::DrawTrailPoints( points, color );
                } )
            .def_static( "DrawAABB",
                         []( const py::array_t<float32>& arr_aabbMin,
                             const py::array_t<float32>& arr_aabbMax,
                             const py::array_t<float32>& arr_aabbWorldTransform,
                             const py::array_t<float32>& arr_color )
                            {
                                CDebugDrawer::DrawAABB( tinymath::nparray_to_vector<float32,3>( arr_aabbMin ),
                                                        tinymath::nparray_to_vector<float32,3>( arr_aabbMax ),
                                                        tinymath::nparray_to_matrix<float32,4>( arr_aabbWorldTransform ),
                                                        tinymath::nparray_to_vector<float32,3>( arr_color ) );
                            } )
            .def_static( "DrawNormals",
                         []( CMesh* meshRef, const py::array_t<float32>& arr_color )
                            {
                                CDebugDrawer::DrawNormals( meshRef, tinymath::nparray_to_vector<float32,3>( arr_color ) );
                            } )
            .def_static( "DrawAxes",
                         []( const py::array_t<float32>& arr_xAxis,
                             const py::array_t<float32>& arr_yAxis,
                             const py::array_t<float32>& arr_zAxis,
                             const py::array_t<float32>& arr_position,
                             float32 size )
                            {
                                CDebugDrawer::DrawAxes( tinymath::nparray_to_vector<float32,3>( arr_xAxis ),
                                                        tinymath::nparray_to_vector<float32,3>( arr_yAxis ),
                                                        tinymath::nparray_to_vector<float32,3>( arr_zAxis ),
                                                        tinymath::nparray_to_vector<float32,3>( arr_position ),
                                                        size );
                            } )
            .def_static( "DrawFrame",
                         []( const py::array_t<float32>& arr_frame,
                             float32 size )
                            {
                                CDebugDrawer::DrawFrame( tinymath::nparray_to_matrix<float32,4>( arr_frame ), size );
                            } )
            .def_static( "DrawPlane", &CDebugDrawer::DrawPlane )
            .def_static( "DrawSolidBox",
                         []( const py::array_t<float32>& arr_size,
                             const py::array_t<float32>& arr_transform,
                             const py::array_t<float32>& arr_rgba )
                            {
                                CDebugDrawer::DrawSolidBox( tinymath::nparray_to_vector<float32,3>( arr_size ),
                                                            tinymath::nparray_to_matrix<float32,4>( arr_transform ),
                                                            tinymath::nparray_to_vector<float32,4>( arr_rgba ) );
                            } )
            .def_static( "DrawSolidSphere",
                         []( float32 radius,
                             const py::array_t<float32>& arr_transform,
                             const py::array_t<float32>& arr_rgba )
                            {
                                CDebugDrawer::DrawSolidSphere( radius,
                                                               tinymath::nparray_to_matrix<float32,4>( arr_transform ),
                                                               tinymath::nparray_to_vector<float32,4>( arr_rgba ) );
                            } )
            .def_static( "DrawSolidCylinder",
                         []( float32 radius, float32 height, const eAxis& axis,
                             const py::array_t<float32>& arr_transform,
                             const py::array_t<float32>& arr_rgba )
                            {
                                CDebugDrawer::DrawSolidCylinder( radius, height, axis,
                                                                 tinymath::nparray_to_matrix<float32,4>( arr_transform ),
                                                                 tinymath::nparray_to_vector<float32,4>( arr_rgba ) );
                            } )
            .def_static( "DrawSolidCapsule",
                         []( float32 radius, float32 height, const eAxis& axis,
                             const py::array_t<float32>& arr_transform,
                             const py::array_t<float32>& arr_rgba )
                            {
                                CDebugDrawer::DrawSolidCapsule( radius, height, axis,
                                                                tinymath::nparray_to_matrix<float32,4>( arr_transform ),
                                                                tinymath::nparray_to_vector<float32,4>( arr_rgba ) );
                            } )
            .def_static( "DrawSolidArrow",
                         []( float32 length, const eAxis& axis,
                             const py::array_t<float32>& arr_transform,
                             const py::array_t<float32>& arr_rgba )
                            {
                                CDebugDrawer::DrawSolidArrow( length, axis,
                                                              tinymath::nparray_to_matrix<float32,4>( arr_transform ),
                                                              tinymath::nparray_to_vector<float32,4>( arr_rgba ) );
                            } )
            .def_static( "DrawSolidAxes",
                         []( float32 length,
                             const py::array_t<float32>& arr_transform,
                             float32 alpha )
                            {
                                CDebugDrawer::DrawSolidAxes( length,
                                                             tinymath::nparray_to_matrix<float32,4>( arr_transform ),
                                                             alpha );
                            } );
    }
}