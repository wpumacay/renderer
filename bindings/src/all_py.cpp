
#include <all_py.h>

// resources required for initialization
#include <utils/CLogger.h>

PYBIND11_MODULE( tinyrenderer, m )
{
    // common bindings
    engine::bindings_common( m );
    // window bindings
    engine::bindings_window( m );

    // execute some required initialization steps
    engine::CLogger::Init();
}