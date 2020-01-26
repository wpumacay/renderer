
#include <all_py.h>

// resources required for initialization
#include <utils/CLogger.h>

PYBIND11_MODULE( tinyrenderer, m )
{
    py::module::import( "tinymath" );

    // common bindings
    engine::bindings_common( m );
    // window bindings
    engine::bindings_window( m );
    // input bindings
    engine::bindings_inputKey( m );
    engine::bindings_inputMouse( m );
    engine::bindings_inputManager( m );
    // buffers bindings
    engine::bindings_bufferLayout( m );
    engine::bindings_vertexBuffer( m );
    engine::bindings_indexBuffer( m );
    engine::bindings_vertexArray( m );
    // shader bindings
    engine::bindings_shader( m );
    engine::bindings_shaderManager( m );
    // texture bindings
    engine::bindings_texture( m );
    engine::bindings_textureManager( m );
    // application bindings
    engine::bindings_application( m );

    // execute some required initialization steps
    engine::CLogger::Init();
}