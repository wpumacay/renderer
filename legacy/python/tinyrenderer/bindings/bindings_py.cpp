
#include <common_py.h>

namespace py = pybind11;

namespace engine
{
    extern void bindings_common( py::module& m );
    extern void bindings_math( py::module& m );
    extern void bindings_window( py::module& m );
    extern void bindings_shaders( py::module& m );
}

PYBIND11_MODULE( tinyrenderer_core, m )
{
    py::module::import( "tinymath" );

    engine::bindings_common( m );
    engine::bindings_math( m );
    engine::bindings_window( m );
    engine::bindings_shaders( m );

    // Initialize helper utilities
    tinyutils::Logger::Init();
    tinyutils::Clock::Init();
    tinyutils::PerlinNoise::Init();
    tinyutils::Profiler::Init();
}
