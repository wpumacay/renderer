
#include <material_py.h>

namespace py = pybind11;

namespace engine
{
    void bindings_material( py::module& m )
    {
        py::enum_< engine::eMaterialType >( m, "MaterialType", py::arithmetic() )
            .value( "NONE", engine::eMaterialType::NONE )
            .value( "LAMBERT", engine::eMaterialType::LAMBERT )
            .value( "PHONG", engine::eMaterialType::PHONG )
            .value( "BLINN_PHONG", engine::eMaterialType::BLINN_PHONG );

        py::class_< CMaterial >( m, "Material" )
            .def( py::init( []( const std::string& name,
                                const eMaterialType& type,
                                const py::array_t<float32>& arr_ambient,
                                const py::array_t<float32>& arr_diffuse,
                                const py::array_t<float32>& arr_specular,
                                float32 shininess,
                                CTexture* albedoMap,
                                CTexture* specularMap )
                    {
                        return std::make_unique<CMaterial>( name,
                                                            type,
                                                            tinymath::nparray_to_vector<float32,3>( arr_ambient ),
                                                            tinymath::nparray_to_vector<float32,3>( arr_diffuse ),
                                                            tinymath::nparray_to_vector<float32,3>( arr_specular ),
                                                            shininess,
                                                            albedoMap,
                                                            specularMap );
                    } ), 
                  py::arg( "name" ), py::arg( "type" ), 
                  py::arg( "ambient" ), py::arg( "diffuse" ), py::arg( "specular" ),
                  py::arg( "shininess" ), py::arg( "albedoMap" ), py::arg( "specularMap" ) )
            .def( "bind", &CMaterial::bind, py::arg( "shader" ) )
            .def( "unbind", &CMaterial::unbind )
            .def_property( "type", &CMaterial::type, &CMaterial::setType )
            .def_property( "albedoMap",
                           &CMaterial::albedoMap,
                           &CMaterial::setAlbedoMap,
                           py::return_value_policy::reference )
            .def_property( "specularMap",
                           &CMaterial::specularMap,
                           &CMaterial::setSpecularMap,
                           py::return_value_policy::reference )
            .def_property_readonly( "name", &CMaterial::name )
            .def_property( "ambient",
                            []( const CMaterial* self ) -> py::array_t<float32>
                                {
                                    return tinymath::vector_to_nparray<float32,3>( self->ambient );
                                },
                            []( CMaterial* self, const py::array_t<float32>& arr_ambient ) -> void
                                {
                                    self->ambient = tinymath::nparray_to_vector<float32,3>( arr_ambient );
                                } )
            .def_property( "diffuse",
                            []( const CMaterial* self ) -> py::array_t<float32>
                                {
                                    return tinymath::vector_to_nparray<float32,3>( self->diffuse );
                                },
                            []( CMaterial* self, const py::array_t<float32>& arr_diffuse ) -> void
                                {
                                    self->diffuse = tinymath::nparray_to_vector<float32,3>( arr_diffuse );
                                } )
            .def_property( "specular",
                            []( const CMaterial* self ) -> py::array_t<float32>
                                {
                                    return tinymath::vector_to_nparray<float32,3>( self->specular );
                                },
                            []( CMaterial* self, const py::array_t<float32>& arr_specular ) -> void
                                {
                                    self->specular = tinymath::nparray_to_vector<float32,3>( arr_specular );
                                } )
            .def_readwrite( "shininess", &CMaterial::shininess )
            .def_readwrite( "transparent", &CMaterial::transparent )
            .def_readwrite( "alpha", &CMaterial::alpha )
            .def( "__repr__", []( const CMaterial* self )
                {
                    auto _strrep = std::string( "Material(\n" );
                    _strrep += "cpp-address : " + engine::pointerToHexAddress( self ) + "\n";
                    _strrep += self->toString();
                    _strrep += ")";
                    return _strrep;
                } );

    }
}