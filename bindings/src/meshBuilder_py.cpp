
#include <meshBuilder_py.h>

namespace py = pybind11;

namespace engine
{
    void bindings_meshBuilder( py::module& m )
    {
        py::class_< CMeshBuilder >( m, "MeshBuilder" )
            .def_static( "CreatePlane",
                         &CMeshBuilder::createPlane,
                         py::arg( "sizeX" ), py::arg( "sizeY" ), py::arg( "axis" ) = eAxis::Z )
            .def_static( "CreateBox",
                         &CMeshBuilder::createBox,
                         py::arg( "sizeX" ), py::arg( "sizeY" ), py::arg( "sizeZ" ) )
            .def_static( "CreateSphere",
                         &CMeshBuilder::createSphere,
                         py::arg( "radius" ), py::arg( "nDiv1" ) = 20, py::arg( "nDiv2" ) = 20 )
            .def_static( "CreateEllipsoid",
                         &CMeshBuilder::createEllipsoid,
                         py::arg( "radX" ), py::arg( "radY" ), py::arg( "radZ" ),
                         py::arg( "nDiv1" ) = 20, py::arg( "nDiv2" ) = 20 )
            .def_static( "CreateCylinder",
                         &CMeshBuilder::createCylinder,
                         py::arg( "radius" ), py::arg( "height" ), py::arg( "axis" ) = eAxis::Z,
                         py::arg( "nDiv1" ) = 30 )
            .def_static( "CreateCapsule",
                         &CMeshBuilder::createCapsule,
                         py::arg( "radius" ), py::arg( "height" ), py::arg( "axis" ) = eAxis::Z,
                         py::arg( "nDiv1" ) = 30, py::arg( "nDiv2" ) = 30 )
            .def_static( "CreateArrow",
                         &CMeshBuilder::createArrow,
                         py::arg( "length" ), py::arg( "axis" ) = eAxis::Z )
            .def_static( "CreateAxes",
                         &CMeshBuilder::createAxes,
                         py::arg( "length" ) )
            .def_static( "CreatePerlinPatch",
                         &CMeshBuilder::createPerlinPatch,
                         py::arg( "width" ), py::arg( "depth" ), py::arg( "cellDivision" ),
                         py::arg( "nOctaves" ) = 4, py::arg( "nPersistance" ) = 0.5f,
                         py::arg( "nLacunarity" ) = 2.0f, py::arg( "nScale" ) = 10.0f,
                         py::arg( "axis" ) = eAxis::Z )
            .def_static( "CreateHeightField",
                         []( int nWidthSamples, int nDepthSamples,
                             float widthExtent, float depthExtent,
                             float centerX, float centerY,
                             const py::array_t<float>& arr_heightData, float heightBase,
                             const eAxis& axis ) -> std::unique_ptr<CMesh>
                            {
                                auto bufferInfo = arr_heightData.request();
                                const int num_points = nWidthSamples * nDepthSamples;
                                const bool columnMajor = bufferInfo.strides[0] < bufferInfo.strides[1];
                                const float* bufferData = (float*) bufferInfo.ptr;
                                if ( bufferInfo.size != num_points )
                                    throw std::runtime_error( "MeshBuilder::CreateHeightField >>> mismatch with the number of points and size of the buffer." );

                                auto heightData = std::vector<float>( num_points, 0.0f );
                                // @optimize: check if matrix traversal is the appropriate one for best cache usage
                                for ( size_t i = 0; i < nDepthSamples; i++ )
                                {
                                    for ( size_t j = 0; j < nWidthSamples; j++ )
                                    {
                                        // heightfield data is expected in row-major order
                                        if ( columnMajor )
                                            heightData[j + i * nWidthSamples] = bufferData[i + j * nDepthSamples];
                                        else
                                            heightData[j + i * nWidthSamples] = bufferData[j + i * nWidthSamples];
                                    }
                                }

                                return std::move( CMeshBuilder::createHeightField( nWidthSamples, nDepthSamples,
                                                                                   widthExtent, depthExtent,
                                                                                   centerX, centerY,
                                                                                   heightData, heightBase,
                                                                                   axis ) );
                            },
                            py::arg( "nWidthSamples" ), py::arg( "nDepthSamples" ),
                            py::arg( "widthExtent" ), py::arg( "depthExtent" ),
                            py::arg( "centerX" ), py::arg( "centerY" ),
                            py::arg( "heightData" ), py::arg( "heightBase" ),
                            py::arg( "axis" ) )
            .def_static( "CreateModelFromFile",
                         &CMeshBuilder::createModelFromFile,
                         py::arg( "filename" ) );
    }
}