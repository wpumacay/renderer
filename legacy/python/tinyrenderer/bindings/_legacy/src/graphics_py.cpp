
#include <graphics_py.h>

namespace py = pybind11;

namespace engine
{
    void bindings_fog( py::module& m )
    {
        py::enum_< engine::eFogType >( m, "FogType", py::arithmetic() )
            .value( "NONE", engine::eFogType::NONE )
            .value( "LINEAR", engine::eFogType::LINEAR )
            .value( "EXPONENTIAL", engine::eFogType::EXPONENTIAL );

        py::class_< CFog >( m, "Fog" )
            .def( py::init( []( const eFogType& type,
                                const py::array_t<float32>& color,
                                float32 density,
                                float32 gradient,
                                float32 distStart,
                                float32 distEnd )
                    {
                        return std::make_unique<CFog>( type,
                                                       tinymath::nparray_to_vector<float32,3>( color ),
                                                       density,
                                                       gradient,
                                                       distStart,
                                                       distEnd );
                    } ),
                  py::arg( "type" ), py::arg( "color" ), py::arg( "density" ), py::arg( "gradient" ),
                  py::arg( "distStart" ), py::arg( "distEnd" ) )
            .def( "bind", &CFog::bind, py::arg( "shader" ) )
            .def( "unbind", &CFog::unbind )
            .def_property( "type", &CFog::type, &CFog::setType )
            .def_property( "active", &CFog::active, &CFog::setActive )
            .def_property( "color",
                           []( const CFog* self ) -> py::array_t<float32>
                                {
                                    return tinymath::vector_to_nparray<float32,3>( self->color );
                                },
                           []( CFog* self, const py::array_t<float32>& arr_color ) -> void
                                {
                                    self->color = tinymath::nparray_to_vector<float32,3>( arr_color );
                                } )
            .def_readwrite( "density", &CFog::density )
            .def_readwrite( "gradient", &CFog::gradient )
            .def_readwrite( "distStart", &CFog::distStart )
            .def_readwrite( "distEnd", &CFog::distEnd )
            .def( "__repr__", []( const CFog* self )
                {
                    auto _strrep = std::string( "Fog(\n" );
                    _strrep += "cpp-address : " + tinyutils::PointerToHexAddress( self ) + "\n";
                    _strrep += self->toString() + ")";
                    return _strrep;
                } );
    }

    void bindings_skybox( py::module& m )
    {
        py::class_< CSkybox >( m, "Skybox" )
            .def( py::init<>() )
            .def( "render", &CSkybox::render )
            .def_property( "cubemap",
                           []( const CSkybox* self ) -> CTextureCube*
                                {
                                    return self->cubemap();
                                },
                           []( CSkybox* self, CTextureCube* cubemapRef ) -> void
                                {
                                    self->setCubemap( cubemapRef );
                                },
                           py::return_value_policy::reference )
            .def( "__repr__", []( const CSkybox* self )
                {
                    auto _cubemapRef = self->cubemap();
                    auto _strrep = std::string( "Skybox(\n" );
                    _strrep += "cpp-address     : " + tinyutils::PointerToHexAddress( self ) + "\n";
                    _strrep += "cubemap-name    : " + std::string( _cubemapRef ? _cubemapRef->name() : "None" ) + ")";
                    return _strrep;
                } );
    }

    void bindings_shadowMap( py::module& m )
    {
        py::enum_< engine::eShadowRangeType >( m, "ShadowRangeType", py::arithmetic() )
            .value( "NONE", engine::eShadowRangeType::NONE )
            .value( "AUTOFIX_CAMERA", engine::eShadowRangeType::AUTOFIX_CAMERA )
            .value( "FIXED_USER", engine::eShadowRangeType::FIXED_USER );

        py::class_< CShadowMapRangeConfig >( m, "ShadowMapRangeConfig" )
            .def( py::init<>() )
            .def_readwrite( "type", &CShadowMapRangeConfig::type )
            .def_property( "worldUp",
                           []( const CShadowMapRangeConfig* self ) -> py::array_t<float32>
                                {
                                    return tinymath::vector_to_nparray<float32,3>( self->worldUp );
                                },
                           []( CShadowMapRangeConfig* self, const py::array_t<float32>& arr_worldUp ) -> void
                                {
                                    self->worldUp = tinymath::nparray_to_vector<float32,3>( arr_worldUp );
                                } )
            .def_property( "pointLight",
                           []( const CShadowMapRangeConfig* self ) -> CPointLight*
                                {
                                    return self->pointLightPtr;
                                },
                           []( CShadowMapRangeConfig* self, CPointLight* pointLightRef ) -> void
                                {
                                    self->pointLightPtr = pointLightRef;
                                },
                           py::return_value_policy::reference )
            .def_property( "spotLight",
                           []( const CShadowMapRangeConfig* self ) -> CSpotLight*
                                {
                                    return self->spotLightPtr;
                                },
                           []( CShadowMapRangeConfig* self, CSpotLight* spotLightRef ) -> void
                                {
                                    self->spotLightPtr = spotLightRef;
                                },
                           py::return_value_policy::reference )
            .def_property( "dirLight",
                           []( const CShadowMapRangeConfig* self ) -> CDirectionalLight*
                                {
                                    return self->dirLightPtr;
                                },
                           []( CShadowMapRangeConfig* self, CDirectionalLight* dirLightRef ) -> void
                                {
                                    self->dirLightPtr = dirLightRef;
                                },
                           py::return_value_policy::reference )
            //// configuration used for autofix-camera
            .def_property( "camera",
                           []( const CShadowMapRangeConfig* self ) -> CICamera*
                                {
                                    return self->cameraPtr;
                                },
                           []( CShadowMapRangeConfig* self, CICamera* cameraRef ) -> void
                                {
                                    self->cameraPtr = cameraRef;
                                },
                           py::return_value_policy::reference )
            .def_readwrite( "extraWidth", &CShadowMapRangeConfig::extraWidth )
            .def_readwrite( "extraHeight", &CShadowMapRangeConfig::extraHeight )
            .def_readwrite( "extraDepth", &CShadowMapRangeConfig::extraDepth )
            //// configuration used for fixed-by-user
            .def_property( "focusPoint",
                           []( const CShadowMapRangeConfig* self ) -> py::array_t<float32>
                                {
                                    return tinymath::vector_to_nparray<float32,3>( self->focusPoint );
                                },
                           []( CShadowMapRangeConfig* self, const py::array_t<float32,3>& arr_focusPoint ) -> void
                                {
                                    self->focusPoint = tinymath::nparray_to_vector<float32,3>( arr_focusPoint );
                                } )
            //// used for orthographics projection
            .def_readwrite( "clipSpaceWidth", &CShadowMapRangeConfig::clipSpaceWidth )
            .def_readwrite( "clipSpaceHeight", &CShadowMapRangeConfig::clipSpaceHeight )
            .def_readwrite( "clipSpaceDepth", &CShadowMapRangeConfig::clipSpaceDepth )
            //// used for perspective projection
            .def_readwrite( "clipSpaceZNear", &CShadowMapRangeConfig::clipSpaceZNear )
            .def_readwrite( "clipSpaceZFar", &CShadowMapRangeConfig::clipSpaceZFar )
            .def_readwrite( "clipSpaceFov", &CShadowMapRangeConfig::clipSpaceFov )
            .def( "__repr__", []( const CShadowMapRangeConfig* self )
                {
                    auto _pointLightRef = self->pointLightPtr;
                    auto _spotLightRef = self->spotLightPtr;
                    auto _dirLightRef = self->dirLightPtr;
                    auto _cameraRef = self->cameraPtr;
                    auto _strrep = std::string( "ShadowMapRangeConfig(\n" );
                    _strrep += "cpp-address     : " + tinyutils::PointerToHexAddress( self ) + "\n";
                    _strrep += "type            : " + engine::toString( self->type ) + "\n";
                    _strrep += "worldUp         : " + engine::toString( self->worldUp ) + "\n";
                    _strrep += "pointLight      : " + std::string( _pointLightRef ? _pointLightRef->name() : "None" ) + "\n";
                    _strrep += "spotLight       : " + std::string( _spotLightRef ? _spotLightRef->name() : "None" ) + "\n";
                    _strrep += "dirLight        : " + std::string( _dirLightRef ? _dirLightRef->name() : "None" ) + "\n";
                    _strrep += "camera          : " + std::string( _cameraRef ? _cameraRef->name() : "None" ) + "\n";
                    _strrep += "extraWidth      : " + std::to_string( self->extraWidth ) + "\n";
                    _strrep += "extraHeight     : " + std::to_string( self->extraHeight ) + "\n";
                    _strrep += "extraDepth      : " + std::to_string( self->extraDepth ) + "\n";
                    _strrep += "focusPoint      : " + engine::toString( self->focusPoint ) + "\n";
                    _strrep += "clipSpaceWidth  : " + std::to_string( self->clipSpaceWidth ) + "\n";
                    _strrep += "clipSpaceHeight : " + std::to_string( self->clipSpaceHeight ) + "\n";
                    _strrep += "clipSpaceDepth  : " + std::to_string( self->clipSpaceDepth ) + "\n";
                    _strrep += "clipSpaceZNear  : " + std::to_string( self->clipSpaceZNear ) + "\n";
                    _strrep += "clipSpaceZFar   : " + std::to_string( self->clipSpaceZFar ) + "\n";
                    _strrep += "clipSpaceFov    : " + std::to_string( self->clipSpaceFov ) + "\n";
                    return _strrep;
                } );

        py::class_< CShadowMap >( m, "ShadowMap" )
            .def( py::init<int32, int32>() )
            .def( "bind", &CShadowMap::bind )
            .def( "unbind", &CShadowMap::unbind )
            .def( "setup", &CShadowMap::setup, py::arg( "config" ) )
            //// // @bug: There seems to be an issue when returning reference, as it's trying to construct other references
            //// //       Perhaps this could be solved if pybind has access to the cframebuffer binded classes as well
            //// .def( "frameBuffer",
            ////       []( const CShadowMap* self ) -> CFrameBuffer*
            ////         {
            ////             return self->frameBuffer();
            ////         },
            ////       py::return_value_policy::reference )
            .def_property_readonly( "width", &CShadowMap::width )
            .def_property_readonly( "height", &CShadowMap::height )
            .def( "lightSpaceMatView",
                  []( const CShadowMap* self ) -> py::array_t<float32>
                    {
                        return tinymath::matrix_to_nparray<float32,3>( self->lightSpaceMatView() );
                    } )
            .def( "lightSpaceMatProj",
                  []( const CShadowMap* self ) -> py::array_t<float32>
                    {
                        return tinymath::matrix_to_nparray<float32,3>( self->lightSpaceMatProj() );
                    } )
            .def( "__repr__", []( const CShadowMap* self )
                {
                    auto _strrep = std::string( "ShadowMap(\n" );
                    _strrep += "cpp-address : " + tinyutils::PointerToHexAddress( self ) + "\n";
                    _strrep += "width       : " + std::to_string( self->width() ) + "\n";
                    _strrep += "height      : " + std::to_string( self->height() ) + "\n";
                    _strrep += "lspaceView  :\n" + engine::toString( self->lightSpaceMatView() ) + "\n";
                    _strrep += "lspaceProj  :\n" + engine::toString( self->lightSpaceMatProj() ) + "\n";
                    return _strrep;
                } );
    }
}
