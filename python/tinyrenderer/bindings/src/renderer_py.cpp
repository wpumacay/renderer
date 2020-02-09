
#include <renderer_py.h>

namespace py = pybind11;

namespace engine
{
    void bindings_renderers( py::module& m )
    {
        // Expose enumerators used along render-options
        {
            py::enum_< engine::eRenderMode >( m, "RenderMode" )
                .value( "NONE", engine::eRenderMode::NONE )
                .value( "NORMAL", engine::eRenderMode::NORMAL )
                .value( "DEPTH_ONLY", engine::eRenderMode::DEPTH_ONLY )
                .value( "SEMANTIC_ONLY", engine::eRenderMode::SEMANTIC_ONLY )
                .value( "NO_SUBMIT", engine::eRenderMode::NO_SUBMIT );

            py::enum_< engine::eCullingGeom >( m, "CullingGeom" )
                .value( "NONE", engine::eCullingGeom::NONE )
                .value( "BOUNDING_BOX", engine::eCullingGeom::BOUNDING_BOX )
                .value( "BOUNDING_SPHERE", engine::eCullingGeom::BOUNDING_SPHERE );
        }

        // Expose Render-Options struct
        {
            py::class_< CRenderOptions >( m, "RenderOptions" )
                .def( py::init<>())
                .def_readwrite( "mode", &CRenderOptions::mode )
                .def_readwrite( "useFrustumCulling", &CRenderOptions::useFrustumCulling )
                .def_readwrite( "cullingGeom", &CRenderOptions::cullingGeom )
                .def_readwrite( "useFaceCulling", &CRenderOptions::useFaceCulling )
                .def_readwrite( "useShadowMapping", &CRenderOptions::useShadowMapping )
                .def_readwrite( "redrawShadowMap", &CRenderOptions::redrawShadowMap )
                .def_readwrite( "pcfCount", &CRenderOptions::pcfCount )
                .def_readwrite( "useBlending", &CRenderOptions::useBlending )
                .def_readwrite( "useFog", &CRenderOptions::useFog )
                .def_readwrite( "useSkybox", &CRenderOptions::useSkybox )
                .def_readwrite( "skyboxFogLowerLimit", &CRenderOptions::skyboxFogLowerLimit )
                .def_readwrite( "skyboxFogUpperLimit", &CRenderOptions::skyboxFogUpperLimit )
                .def_readwrite( "viewportWidth", &CRenderOptions::viewportWidth )
                .def_readwrite( "viewportHeight", &CRenderOptions::viewportHeight )
                // @firsttodo: test that ownership is not messed up
                // @firsttodo: check if can change keep_alive<1, 0> of readwrite for only return_value_policy::reference
                .def_readwrite( "shadowMapRangeConfig", &CRenderOptions::shadowMapRangeConfig )
                .def_readwrite( "camera", &CRenderOptions::cameraPtr )
                .def_readwrite( "light", &CRenderOptions::lightPtr )
                .def_readwrite( "shadowMap", &CRenderOptions::shadowMapPtr )
                .def_readwrite( "renderTarget", &CRenderOptions::renderTargetPtr )
                .def_readwrite( "fog", &CRenderOptions::fogPtr )
                .def_readwrite( "skybox", &CRenderOptions::skyboxPtr )
                //// .def_property( "camera",
                ////     []( const CRenderOptions* self ) -> CICamera*
                ////         {
                ////             return self->cameraPtr;
                ////         },
                ////     []( CRenderOptions* self, CICamera* cameraRef ) -> void
                ////         {
                ////             self->cameraPtr = cameraRef;
                ////         },
                ////     py::return_value_policy::reference )
                //// .def_property( "light",
                ////     []( const CRenderOptions* self ) ->  CILight*
                ////         {
                ////             return self->lightPtr;
                ////         },
                ////     []( CRenderOptions* self, CILight* lightRef ) -> void
                ////         {
                ////             self->lightPtr = lightRef;
                ////         },
                ////     py::return_value_policy::reference )
                //// .def_property( "shadowMap",
                ////     []( const CRenderOptions* self ) -> CShadowMap*
                ////         {
                ////             return self->shadowMapPtr;
                ////         },
                ////     []( CRenderOptions* self, CShadowMap* shadowMapRef ) -> void
                ////         {
                ////             self->shadowMapPtr = shadowMapRef;
                ////         },
                ////     py::return_value_policy::reference )
                //// .def_property( "renderTarget",
                ////     []( const CRenderOptions* self ) -> CFrameBuffer*
                ////         {
                ////             return self->renderTargetPtr;
                ////         },
                ////     []( CRenderOptions* self, CFrameBuffer* renderTargetRef ) -> void
                ////         {
                ////             self->renderTargetPtr = renderTargetRef;
                ////         },
                ////     py::return_value_policy::reference )
                //// .def_property( "fog",
                ////     []( const CRenderOptions* self ) -> CFog*
                ////         {
                ////             return self->fogPtr;
                ////         },
                ////     []( CRenderOptions* self, CFog* fogRef ) -> void
                ////         {
                ////             self->fogPtr = fogRef;
                ////         },
                ////     py::return_value_policy::reference )
                //// .def_property( "skybox",
                ////     []( const CRenderOptions* self ) -> CSkybox*
                ////         {
                ////             return self->skyboxPtr;
                ////         },
                ////     []( CRenderOptions* self, CSkybox* skyboxRef ) -> void
                ////         {
                ////             self->skyboxPtr = skyboxRef;
                ////         },
                ////     py::return_value_policy::reference )
                .def_readwrite( "depthViewZmin", &CRenderOptions::depthViewZmin )
                .def_readwrite( "depthViewZmax", &CRenderOptions::depthViewZmax )
                .def_property( "depthViewZminColor",
                    []( const CRenderOptions* self ) -> py::array_t<float32>
                        {
                            return tinymath::vector_to_nparray<float32, 3>( self->depthViewZminColor );
                        },
                    []( CRenderOptions* self, const py::array_t<float32>& arr_depthViewZminColor ) -> void
                        {
                            self->depthViewZminColor = tinymath::nparray_to_vector<float32, 3>( arr_depthViewZminColor );
                        } )
                .def_property( "depthViewZmaxColor",
                    []( const CRenderOptions* self ) -> py::array_t<float32>
                        {
                            return tinymath::vector_to_nparray<float32, 3>( self->depthViewZmaxColor );
                        },
                    []( CRenderOptions* self, const py::array_t<float32>& arr_depthViewZmaxColor ) -> void
                        {
                            self->depthViewZmaxColor = tinymath::nparray_to_vector<float32, 3>( arr_depthViewZmaxColor );
                        } )
                .def( "__repr__", []( const CRenderOptions* self )
                    {
                        auto _strrep = std::string( "RenderOptions(\n" );
                        _strrep += "cpp-address     : " + engine::pointerToHexAddress( self ) + "\n";
                        _strrep += engine::toString( *self );
                        _strrep += ")";
                        return _strrep;
                    } );
        }

        // Expose renderers
        {
            // @todo: expose mesh-renderer
            // @todo: expose skybox-renderer
            // @todo: expose main-renderer
        }
    }
}