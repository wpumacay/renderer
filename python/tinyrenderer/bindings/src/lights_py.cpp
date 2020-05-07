
#include <lights_py.h>

namespace py = pybind11;

namespace engine
{
    void bindings_lightBase( py::module& m )
    {
        py::enum_<engine::eLightType>( m, "LightType", py::arithmetic() )
            .value( "NONE", engine::eLightType::NONE )
            .value( "DIRECTIONAL", engine::eLightType::DIRECTIONAL )
            .value( "POINT", engine::eLightType::POINT )
            .value( "SPOT", engine::eLightType::SPOT );

        py::class_< CILight >( m, "ILight" )
            //// Instance variables common to all light-types
            .def_readwrite( "enabled", &CILight::enabled )
            .def_property( "ambient",
                           []( const CILight* self ) -> py::array_t<float32>
                           {
                                return tinymath::vector_to_nparray<float32,3>( self->ambient );
                           },
                           []( CILight* self, const py::array_t<float32>& arr_ambient )
                           {
                                self->ambient = tinymath::nparray_to_vector<float32,3>( arr_ambient );
                           } )
            .def_property( "diffuse",
                           []( const CILight* self ) -> py::array_t<float32>
                           {
                                return tinymath::vector_to_nparray<float32,3>( self->diffuse );
                           },
                           []( CILight* self, const py::array_t<float32>& arr_diffuse )
                           {
                                self->diffuse = tinymath::nparray_to_vector<float32,3>( arr_diffuse );
                           } )
            .def_property( "specular",
                           []( const CILight* self ) -> py::array_t<float32>
                           {
                                return tinymath::vector_to_nparray<float32,3>( self->specular );
                           },
                           []( CILight* self, const py::array_t<float32>& arr_specular )
                           {
                                self->specular = tinymath::nparray_to_vector<float32,3>( arr_specular );
                           } )
            .def_readwrite( "intensity", &CILight::intensity )
            .def_readwrite( "producesShadows", &CILight::producesShadows )
            //// Instance variables common to specific light-types
            .def_property( "direction",
                           []( const CILight* self ) -> py::array_t<float32>
                           {
                                return tinymath::vector_to_nparray<float32,3>( self->direction );
                           },
                           []( CILight* self, const py::array_t<float32,3>& arr_direction )
                           {
                                self->direction = tinymath::nparray_to_vector<float32,3>( arr_direction );
                           } )
            .def_property( "position",
                           []( const CILight* self ) -> py::array_t<float32>
                           {
                                return tinymath::vector_to_nparray<float32,3>( self->position );
                           },
                           []( CILight* self, const py::array_t<float32,3>& arr_position )
                           {
                                self->position = tinymath::nparray_to_vector<float32,3>( arr_position );
                           } )
            .def_readwrite( "atnConstant", &CILight::atnConstant )
            .def_readwrite( "atnLinear", &CILight::atnLinear )
            .def_readwrite( "atnQuadratic", &CILight::atnQuadratic )
            .def_readwrite( "innerCutoff", &CILight::innerCutoff )
            .def_readwrite( "outerCutoff", &CILight::outerCutoff )
            .def_property_readonly( "name", &CILight::name )
            .def_property_readonly( "type", &CILight::type )
            .def( "__repr__", []( const CILight* self )
                {
                    auto _strrep = std::string( "Light(\n" );
                    _strrep += "cpp-address     : " + tinyutils::PointerToHexAddress( self ) + "\n";
                    _strrep += self->toString();
                    _strrep += ")";
                    return _strrep;
                } );
    }

    void bindings_lightDirectional( py::module& m )
    {
        py::class_< CDirectionalLight, CILight >( m, "DirectionalLight" )
            .def( py::init( []( const std::string& name,
                                const py::array_t<float32>& arr_ambientColor,
                                const py::array_t<float32>& arr_diffuseColor,
                                const py::array_t<float32>& arr_specularColor,
                                const py::array_t<float32>& arr_lightDirection )
                {
                    return std::make_unique<CDirectionalLight>( name,
                                                                tinymath::nparray_to_vector<float32,3>( arr_ambientColor ),
                                                                tinymath::nparray_to_vector<float32,3>( arr_diffuseColor ),
                                                                tinymath::nparray_to_vector<float32,3>( arr_specularColor ),
                                                                tinymath::nparray_to_vector<float32,3>( arr_lightDirection ) );
                } ) );
    }

    void bindings_lightPoint( py::module& m )
    {
        py::class_< CPointLight, CILight >( m, "PointLight" )
            .def( py::init( []( const std::string& name,
                                const py::array_t<float32>& arr_ambientColor,
                                const py::array_t<float32>& arr_diffuseColor,
                                const py::array_t<float32>& arr_specularColor,
                                const py::array_t<float32>& arr_lightPosition,
                                float32 attenuationFactorConstant,
                                float32 attenuationFactorLinear,
                                float32 attenuationFactorQuadratic )
                {
                    return std::make_unique<CPointLight>( name,
                                                          tinymath::nparray_to_vector<float32,3>( arr_ambientColor ),
                                                          tinymath::nparray_to_vector<float32,3>( arr_diffuseColor ),
                                                          tinymath::nparray_to_vector<float32,3>( arr_specularColor ),
                                                          tinymath::nparray_to_vector<float32,3>( arr_lightPosition ),
                                                          attenuationFactorConstant,
                                                          attenuationFactorLinear,
                                                          attenuationFactorQuadratic );
                } ) );
    }

    void bindings_lightSpot( py::module& m )
    {
        py::class_< CSpotLight, CILight >( m, "SpotLight" )
            .def( py::init( []( const std::string& name,
                                const py::array_t<float32>& arr_ambientColor,
                                const py::array_t<float32>& arr_diffuseColor,
                                const py::array_t<float32>& arr_specularColor,
                                const py::array_t<float32>& arr_lightPosition,
                                const py::array_t<float32>& arr_lightDirection,
                                float32 attenuationFactorConstant,
                                float32 attenuationFactorLinear,
                                float32 attenuationFactorQuadratic,
                                float32 innerCutoff,
                                float32 outerCutoff )
                {
                    return std::make_unique<CSpotLight>( name,
                                                         tinymath::nparray_to_vector<float32,3>( arr_ambientColor ),
                                                         tinymath::nparray_to_vector<float32,3>( arr_diffuseColor ),
                                                         tinymath::nparray_to_vector<float32,3>( arr_specularColor ),
                                                         tinymath::nparray_to_vector<float32,3>( arr_lightPosition ),
                                                         tinymath::nparray_to_vector<float32,3>( arr_lightDirection ),
                                                         attenuationFactorConstant,
                                                         attenuationFactorLinear,
                                                         attenuationFactorQuadratic,
                                                         innerCutoff,
                                                         outerCutoff );
                } ) );
    }
}