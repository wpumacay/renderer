message( "Configuring external dependencies" )

# Configure build-options for spdlog
set( SPDLOG_BUILD_SHARED ON CACHE BOOL "Build shared library" )
set( SPDLOG_BUILD_EXAMPLE OFF CACHE BOOL "Don't build spdlog examples" )
set( SPDLOG_BUILD_EXAMPLE_HO OFF CACHE BOOL "Don't build spdlog examples (ho-flag)" )
set( SPDLOG_BUILD_TESTS OFF CACHE BOOL "Don't build spdlog tests" )
set( SPDLOG_BUILD_TESTS_HO OFF CACHE BOOL "Don't build spdlog tests (ho-flag)" )
set( SPDLOG_BUILD_BENCH OFF CACHE BOOL "Don't build spdlog benchmarks" )

# Configure build-options for tiny_math
set( TINYMATH_BUILD_DOCS OFF CACHE BOOL "Don't build tinymath's documentation" )
set( TINYMATH_BUILD_EXAMPLES OFF CACHE BOOL "Don't build tinymath's examples" )
set( TINYMATH_BUILD_PYTHON_BINDINGS ON CACHE BOOL "Build tinymath's python-bindings" )

# Configure build-options for tiny_utils
set( TINYUTILS_BUILD_EXAMPLES OFF CACHE BOOL "Don't build tinyutils' examples" )
set( TINYUTILS_BUILD_PYTHON_BINDINGS ON CACHE BOOL "Build tinyutils' python-bindings" )

# Configure build-options for ImGui
set( IMGUI_BUILD_EXAMPLES CACHE BOOL "Don't build ImGui examples" )

add_subdirectory( imgui )
add_subdirectory( spdlog )
add_subdirectory( pybind11 )
add_subdirectory( tiny_math )
add_subdirectory( tiny_utils )