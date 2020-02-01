
# Configure dependencies
message( "Configuring external dependencies" )

# build spdlog as a shared library
set( SPDLOG_BUILD_SHARED ON CACHE BOOL "Build shared library" )

# build tiny_math without examples
set( TINYMATH_BUILD_DOCS OFF CACHE BOOL "Don't build tinymath's documentation" )
set( TINYMATH_BUILD_EXAMPLES OFF CACHE BOOL "Don't build tinymath's examples" )
set( TINYMATH_BUILD_PYTHON_BINDINGS ON CACHE BOOL "Build tinymath's python-bindings" )

if ( NOT TINYRENDERER_BUILD_HEADLESS_EGL )
add_subdirectory( imgui )
endif()

add_subdirectory( spdlog )
add_subdirectory( pybind11 )
add_subdirectory( tiny_math )