message( "Configuring external dependencies" )

# build spdlog as a shared library
set( SPDLOG_BUILD_SHARED ON CACHE BOOL "Build shared library" )
set( SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "Don't build spdlog examples" )
set( SPDLOG_BUILD_EXAMPLE_HO OFF CACHE BOOL "Don't build spdlog examples (ho-flag)" )
set( SPDLOG_BUILD_TESTS OFF CACHE BOOL "Don't build spdlog tests" )
set( SPDLOG_BUILD_TESTS_HO OFF CACHE BOOL "Don't build spdlog tests (ho-flag)" )
set( SPDLOG_BUILD_BENCH OFF CACHE BOOL "Don't build spdlog benchmarks" )

# build tiny_math without examples
set( TINYMATH_BUILD_DOCS OFF CACHE BOOL "Don't build tinymath's documentation" )
set( TINYMATH_BUILD_EXAMPLES OFF CACHE BOOL "Don't build tinymath's examples" )
set( TINYMATH_BUILD_PYTHON_BINDINGS ON CACHE BOOL "Build tinymath's python-bindings" )

# build ImGui without examples
set( IMGUI_BUILD_EXAMPLES CACHE BOOL "Don't build ImGui examples" )

add_subdirectory( imgui )
add_subdirectory( spdlog )
add_subdirectory( pybind11 )
add_subdirectory( tiny_math )