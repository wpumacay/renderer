
#include <common_py.h>

namespace py = pybind11;

namespace engine
{
    extern void bindings_common( py::module& m );
    extern void bindings_math( py::module& m );
    extern void bindings_window( py::module& m );
}

#ifndef ENGINE_HEADLESS_EGL
PYBIND11_MODULE( tinyrenderer_core, m )
#else
PYBIND11_MODULE( tinyrenderer_core_egl, m )
#endif
{
    py::module::import( "tinymath" );

    engine::bindings_common( m );
    engine::bindings_math( m );
    engine::bindings_window( m );

    // Initialize helper utilities
    tinyutils::Logger::Init();
    tinyutils::Clock::Init();
    tinyutils::PerlinNoise::Init();
    tinyutils::Profiler::Init();
}