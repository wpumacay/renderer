
#include <all_py.h>

#ifndef ENGINE_HEADLESS_EGL
PYBIND11_MODULE( tinyrenderer_core, m )
#else
PYBIND11_MODULE( tinyrenderer_core_egl, m )
#endif
{
    py::module::import( "tinymath" );

    // common bindings
    engine::bindings_common( m );
    // math-helpers bindings
    engine::bindings_mathHelpers( m );
    // window bindings
    engine::bindings_windowBase( m );
#ifndef ENGINE_HEADLESS_EGL
    engine::bindings_windowGLFW( m );
#else
    engine::bindings_windowEGL( m );
#endif /* ENGINE_HEADLESS_EGL */
    // input bindings
    engine::bindings_inputKey( m );
    engine::bindings_inputMouse( m );
    engine::bindings_inputManager( m );
    // buffers bindings
    engine::bindings_bufferLayout( m );
    engine::bindings_vertexBuffer( m );
    engine::bindings_indexBuffer( m );
    engine::bindings_vertexArray( m );
    // frameBuffer bindings
    engine::bindings_frameBuffer( m );
    // shader bindings
    engine::bindings_shader( m );
    engine::bindings_shaderManager( m );
    // texture bindings
    engine::bindings_texture( m );
    engine::bindings_textureCube( m );
    engine::bindings_textureManager( m );
    // material bindings
    engine::bindings_material( m );
    // camera bindings
    engine::bindings_cameraBase( m );
    engine::bindings_cameraTypes( m );
    // graphics bindings
    engine::bindings_fog( m );
    engine::bindings_skybox( m );
    engine::bindings_shadowMap( m );
    // light bindings
    engine::bindings_lightBase( m );
    engine::bindings_lightDirectional( m );
    engine::bindings_lightPoint( m );
    engine::bindings_lightSpot( m );
    // debug bindings
    engine::bindings_debugDrawer( m );
    // scene bindings
    engine::bindings_scene( m );
    // renderable bindings
    engine::bindings_renderableBase( m );
    engine::bindings_renderableMesh( m );
    engine::bindings_renderableModel( m );
    // mesh-builder bindings
    engine::bindings_meshBuilder( m );
    // renderers bindings
    engine::bindings_renderers( m );
    // application bindings
    engine::bindings_application( m );

    // expose some #define(s)
    m.attr( "ENGINE_EXAMPLES_PATH" ) = py::cast( ENGINE_EXAMPLES_PATH );
    m.attr( "ENGINE_RESOURCES_PATH" ) = py::cast( ENGINE_RESOURCES_PATH );

    // execute some required initialization steps
    tinyutils::Logger::Init();
    tinyutils::Clock::Init();
    tinyutils::PerlinNoise::Init();
    tinyutils::Profiler::Init();
}