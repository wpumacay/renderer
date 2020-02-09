
#include <utils_py.h>

namespace py = pybind11;

namespace engine
{
    void bindings_utilTime( py::module& m )
    {
        py::class_< CTime >( m, "Time" )
            .def_static( "Init", &CTime::Init )
            .def_static( "Release", &CTime::Release )
            .def_static( "GetTimeStep", &CTime::GetTimeStep )
            .def_static( "GetRawTimeStep", &CTime::GetRawTimeStep )
            .def_static( "GetAvgTimeStep", &CTime::GetAvgTimeStep )
            .def_static( "GetAvgFps", &CTime::GetAvgFps )
            .def_static( "GetWallTime", &CTime::GetWallTime )
            .def_static( "GetFrameTimes", &CTime::GetFrameTimes )
            .def_static( "GetFpsAvgs", &CTime::GetFpsAvgs )
            .def_static( "GetNumFramesForAvg", &CTime::GetNumFramesForAvg )
            .def_static( "GetFrameTimeIndex", &CTime::GetFrameTimeIndex );
    }
}