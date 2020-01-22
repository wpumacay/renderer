
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
    // shader bindings
    engine::bindings_shader( m );
    engine::bindings_shaderManager( m );
    // application bindings
    engine::bindings_application( m );

    // execute some required initialization steps
    engine::CLogger::Init();
}