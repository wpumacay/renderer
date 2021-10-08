
#include <renderable_py.h>

namespace py = pybind11;

namespace engine
{
    void bindings_renderableBase( py::module& m )
    {
        py::class_< CIRenderable >( m, "IRenderable" )
            .def( "render", &CIRenderable::render )
            .def( "bbox", &CIRenderable::bbox )
            .def( "bsphere", &CIRenderable::bsphere )
            .def( "matModel", &CIRenderable::matModel )
            .def_property( "material",
                []( const CIRenderable* self ) -> CMaterial*
                    {
                        return self->material();
                    },
                []( CIRenderable* self, std::unique_ptr<CMaterial> material ) -> void
                    {
                        self->setMaterial( std::move( material ) );
                    }, py::return_value_policy::reference, py::keep_alive<1, 2>() )
            .def_property( "position",
                []( const CIRenderable* self ) -> py::array_t<float32>
                    {
                        return tinymath::vector_to_nparray<float32,3>( self->position );
                    },
                []( CIRenderable* self, const py::array_t<float32>& arr_position )
                    {
                        self->position = tinymath::nparray_to_vector<float32,3>( arr_position );
                    } )
            .def_property( "rotation",
                []( const CIRenderable* self ) -> py::array_t<float32>
                    {
                        return tinymath::matrix_to_nparray<float32,3>( self->rotation );
                    },
                []( CIRenderable* self, const py::array_t<float32>& arr_rotation )
                    {
                        self->rotation = tinymath::nparray_to_matrix<float32,3>( arr_rotation );
                    } )
            .def_property( "scale",
                []( const CIRenderable* self ) -> py::array_t<float32>
                    {
                        return tinymath::vector_to_nparray<float32,3>( self->scale );
                    },
                []( CIRenderable* self, const py::array_t<float32>& arr_scale )
                    {
                        self->scale = tinymath::nparray_to_vector<float32,3>( arr_scale );
                    } )
            .def_readwrite( "castsShadows", &CIRenderable::castsShadows )
            .def_readwrite( "receivesShadows", &CIRenderable::receivesShadows )
            .def_readwrite( "cullFaces", &CIRenderable::cullFaces )
            .def_property( "visible", &CIRenderable::visible, &CIRenderable::setVisibility )
            .def_property( "wireframe", &CIRenderable::wireframe, &CIRenderable::setWireframe )
            .def_property( "maskId", &CIRenderable::maskId, &CIRenderable::setMaskId )
            .def_property( "maskId", &CIRenderable::objectId, &CIRenderable::setObjectId )
            .def_property_readonly( "name", &CIRenderable::name )
            .def_property_readonly( "type", &CIRenderable::type );
    }

    void bindings_renderableMesh( py::module& m )
    {
        py::class_< CMesh, CIRenderable >( m, "Mesh" )
            .def( py::init( []( const std::string& name,
                                const std::vector<CVec3>& vertices,
                                const std::vector<CVec3>& normals,
                                const std::vector<CVec2>& texCoords,
                                const std::vector<CInd3>& indices )
                {
                    return std::make_unique<CMesh>( name, vertices, normals, texCoords, indices, eBufferUsage::STATIC );
                } ) )
            .def( py::init( []( const std::string& name,
                                const py::array_t<float32>& arr_vertices,
                                const py::array_t<float32>& arr_normals,
                                const py::array_t<float32>& arr_texCoords,
                                const py::array_t<uint32>& arr_indices )
                {
                    auto vertices = std::vector<CVec3>();
                    auto normals = std::vector<CVec3>();
                    auto texCoords = std::vector<CVec2>();
                    auto indices = std::vector<CInd3>();

                    auto arr_vertInfo = arr_vertices.request();
                    auto arr_normInfo = arr_normals.request();
                    auto arr_texInfo = arr_texCoords.request();
                    auto arr_indInfo = arr_indices.request();

                    if ( arr_vertInfo.ndim != 2 || arr_vertInfo.shape[1] != 3 )
                        throw std::runtime_error( "Mesh >>> vertices-buffer given for mesh-data has wrong layout. Expected [batch,3] (2-dims) , but \
                                                   got (" + std::to_string( arr_vertInfo.ndim ) + "-dims) and shape [" + std::to_string( arr_vertInfo.shape[0] ) + "," + std::to_string( arr_vertInfo.shape[1] ) + "]." );

                    if ( arr_normInfo.ndim != 2 || arr_normInfo.shape[1] != 3 )
                        throw std::runtime_error( "Mesh >>> normals-buffer given for mesh-data has wrong layout. Expected [batch,3] (2-dims) , but \
                                                   got (" + std::to_string( arr_normInfo.ndim ) + "-dims) and shape [" + std::to_string( arr_normInfo.shape[0] ) + "," + std::to_string( arr_normInfo.shape[1] ) + "]." );

                    if ( arr_texInfo.ndim != 2 || arr_texInfo.shape[1] != 2 )
                        throw std::runtime_error( "Mesh >>> uvs-buffer given for mesh-data has wrong layout. Expected [batch,2] (2-dims) , but \
                                                   got (" + std::to_string( arr_texInfo.ndim ) + "-dims) and shape [" + std::to_string( arr_texInfo.shape[0] ) + "," + std::to_string( arr_texInfo.shape[1] ) + "]." );

                    if ( arr_indInfo.ndim != 2 || arr_indInfo.shape[1] != 3 )
                        throw std::runtime_error( "Mesh >>> indices-buffer given for mesh-data has wrong layout. Expected [batch,3] (2-dims) , but \
                                                   got (" + std::to_string( arr_indInfo.ndim ) + "-dims) and shape [" + std::to_string( arr_indInfo.shape[0] ) + "," + std::to_string( arr_indInfo.shape[1] ) + "]." );

                    grabBatchOfVector3FromNpArray( arr_vertices, vertices );
                    grabBatchOfVector3FromNpArray( arr_normals, normals );
                    grabBatchOfVector2FromNpArray( arr_texCoords, texCoords );
                    grabBatchOfInd3FromNpArray( arr_indices, indices );

                    return std::make_unique<CMesh>( name, vertices, normals, texCoords, indices, eBufferUsage::STATIC );
                } ) )
            .def( "vertices", []( CMesh* self ) -> std::vector< CVec3 >
                {
                    return self->vertices();
                } )
            .def( "normals", []( CMesh* self ) -> std::vector< CVec3 >
                {
                    return self->normals();
                } )
            .def( "uvs", []( CMesh* self ) -> std::vector< CVec2 >
                {
                    return self->uvs();
                } )
            .def( "indices", []( CMesh* self ) -> std::vector< CInd3 >
                {
                    return self->indices();
                } )
            .def( "vao", []( CMesh* self ) -> CVertexArray*
                {
                    return self->vertexArray().get();
                }, py::return_value_policy::reference );
    }

    void bindings_renderableModel( py::module& m )
    {
        py::class_< CModel, CIRenderable >( m, "Model" )
            .def( py::init<const std::string&>() )
            .def( "addMesh",
                  []( CModel* self, std::unique_ptr<CMesh> mesh,
                      const py::array_t<float32>& arr_localTf )
                    {
                        self->addMesh( std::move( mesh ),
                                       tinymath::nparray_to_matrix<float,4>( arr_localTf ) );
                    }, py::keep_alive<1, 2>() )
            .def( "meshes",
                  []( CModel* self ) -> std::vector< CMesh* >
                    {
                        return self->meshes();
                    },
                  py::return_value_policy::reference )
            .def( "localTransforms",
                  []( CModel* self ) -> std::vector< CMat4 >
                    {
                        return self->localTransforms();
                    } );
    }

    void grabBatchOfVector3FromNpArray( const py::array_t<float32>& src_arr,
                                        std::vector<CVec3>& dst_vec )
    {
        auto bufferInfo = src_arr.request();
        const float32* bufferData = (float32*) bufferInfo.ptr;
        const bool columnMajor = bufferInfo.strides[0] < bufferInfo.strides[1];
        for ( size_t i = 0; i < bufferInfo.shape[0]; i++ )
        {
            CVec3 vec;
            for ( size_t j = 0; j < bufferInfo.shape[1]; j++ )
            {
                if ( columnMajor ) // use this indexing as vectors are in the rows (shape is (batch,3))
                    vec[j] = bufferData[i + j * bufferInfo.shape[0]];
                else
                    vec[j] = bufferData[j + i * bufferInfo.shape[1]];
            }
            dst_vec.push_back( vec );
        }
    }

    void grabBatchOfVector2FromNpArray( const py::array_t<float32>& src_arr,
                                        std::vector<CVec2>& dst_vec )
    {
        auto bufferInfo = src_arr.request();
        const float32* bufferData = (float32*) bufferInfo.ptr;
        const bool columnMajor = bufferInfo.strides[0] < bufferInfo.strides[1];
        for ( size_t i = 0; i < bufferInfo.shape[0]; i++ )
        {
            CVec2 vec;
            for ( size_t j = 0; j < bufferInfo.shape[1]; j++ )
            {
                if ( columnMajor ) // use this indexing as vectors are in the rows (shape is (batch,2))
                    vec[j] = bufferData[i + j * bufferInfo.shape[0]];
                else
                    vec[j] = bufferData[j + i * bufferInfo.shape[1]];
            }
            dst_vec.push_back( vec );
        }
    }

    void grabBatchOfInd3FromNpArray( const py::array_t<uint32>& src_arr,
                                     std::vector<CInd3>& dst_vec )
    {
        auto bufferInfo = src_arr.request();
        const uint32* bufferData = (uint32*) bufferInfo.ptr;
        const bool columnMajor = bufferInfo.strides[0] < bufferInfo.strides[1];
        for ( size_t i = 0; i < bufferInfo.shape[0]; i++ )
        {
            CInd3 ind;
            for ( size_t j = 0; j < bufferInfo.shape[1]; j++ )
            {
                if ( columnMajor ) // use this indexing as vectors are in the rows (shape is (batch,3))
                    ind.buff[j] = bufferData[i + j * bufferInfo.shape[0]];
                else
                    ind.buff[j] = bufferData[j + i * bufferInfo.shape[1]];
            }
            dst_vec.push_back( ind );
        }
    }
}
