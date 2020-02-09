message( "TINYRENDERER::bindings >>> configuring python-bindings" )

file( GLOB_RECURSE BindingsFilesSources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )
include_directories( "${TINYRENDERER_INCLUDES_DIRS}" )
include_directories( "${CMAKE_SOURCE_DIR}/ext/tiny_math/python/include" )

pybind11_add_module( tinyrenderer_core MODULE "${BindingsFilesSources}" )
target_link_libraries( tinyrenderer_core PRIVATE tinyrenderer_cpp_lib tinymath_python_lib )
