message( "TINYRENDERER::bindings >>> configuring python-bindings" )

file( GLOB_RECURSE BindingsFilesSources "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" )
set( BindingsFilesSources
     "${CMAKE_CURRENT_SOURCE_DIR}/common_py.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/math_py.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/window/window_py.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/bindings_py.cpp" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}" )
include_directories( "${TINYRENDERER_INCLUDES_DIRS}" )
include_directories( "${CMAKE_SOURCE_DIR}/ext/tiny_math/python/include" )

if ( TINYRENDERER_BUILD_WINDOWED_GLFW )
    pybind11_add_module( tinyrenderer_core MODULE "${BindingsFilesSources}" )
    target_link_libraries( tinyrenderer_core PRIVATE tinyrenderer_cpp_lib_glfw tinymath_python_lib )
    target_compile_definitions( tinyrenderer_core PRIVATE ENGINE_WINDOWED_GLFW=1 )
endif()

if ( TINYRENDERER_BUILD_HEADLESS_EGL )
    pybind11_add_module( tinyrenderer_core_egl MODULE "${BindingsFilesSources}" )
    target_link_libraries( tinyrenderer_core_egl PRIVATE tinyrenderer_cpp_lib_egl tinymath_python_lib )
    target_compile_definitions( tinyrenderer_core_egl PRIVATE ENGINE_HEADLESS_EGL=1 )
endif()
