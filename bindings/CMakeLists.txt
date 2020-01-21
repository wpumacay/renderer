message( "TINYRENDERER::bindings >>> configuring python-bindings" )

file( GLOB_RECURSE BindingsFilesSources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )
include_directories( "${TINYRENDERER_INCLUDES_DIRS}" )

pybind11_add_module( tinyrenderer MODULE "${BindingsFilesSources}" )
target_link_libraries( tinyrenderer PRIVATE tinyrenderer_cpp_lib )
