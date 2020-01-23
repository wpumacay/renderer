cmake_minimum_required( VERSION 3.0.0 )

# aka cat1-project (=ↀωↀ=)
project( TinyRenderer )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wno-format-security -Wno-endif-labels -fPIC" )

# in case nobody set the project type, set it (parent project might have set it for all)
if ( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Debug )
endif()

# compile everything as shared libraries
set( BUILD_SHARED_LIBS ON CACHE BOOL "Build libraries as shared" )

# outputs of the build should be sent accordingly
if ( NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY )
    message( "TINYRENDERER> Sending generated libs to OWN build directory" )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs" )
else()
    message( "TINYRENDERER> Sending generated libs to EXTERNAL build directory" )
endif()
message( "TINYRENDERER> LIBRARY_OUT_DIR: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" )

set( TINYRENDERER_IS_MASTER_PROJECT OFF )
if( CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( "TINYRENDERER> Configuring as MASTER project" )
    set( TINYRENDERER_IS_MASTER_PROJECT ON )
else()
    message( "TINYRENDERER> Configuring as CHILD project" )
endif()

option( TINYRENDERER_BUILD_DOCS "Build documentation (requires Doxygen)" ON )
option( TINYRENDERER_BUILD_EXAMPLES "Build c/c++ examples" ON )
option( TINYRENDERER_BUILD_PYTHON_BINDINGS "Build python bindings" ON )
option( TINYRENDERER_BUILD_WITH_LOGS "Build using logging functionality" ON )

# Add cmake find-modules helpers (for sphinx)
if( TINYRENDERER_IS_MASTER_PROJECT )
    set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH} )
endif()

# Add external dependencies only if master project. If child project, root adds the dependencies
if ( TINYRENDERER_IS_MASTER_PROJECT )
    add_subdirectory( ext )
endif()

find_package( OpenGL REQUIRED )
find_package( GLEW REQUIRED )
find_package( ASSIMP REQUIRED )
find_package( PkgConfig REQUIRED )
pkg_search_module( GLFW REQUIRED glfw3 )

if ( APPLE )
    include_directories( /usr/local/include )
    link_directories( /usr/local/lib )
endif()

message( "ASSIMP_INCLUDE_DIRS   : ${ASSIMP_INCLUDE_DIRS}" )
message( "GLFW_INCLUDE_DIRS     : ${GLFW_INCLUDE_DIRS}" )
message( "GLFW_LIBRARIES        : ${GLFW_LIBRARIES}" )
message( "GLFW_LIBRARY_DIRS     : ${GLFW_LIBRARY_DIRS}" )
message( "GLEW_INCLUDE_DIRS     : ${GLEW_INCLUDE_DIRS}" )
message( "GLEW_LIBRARIES        : ${GLEW_LIBRARIES}" )
message( "OPENGL_LIBRARIES      : ${OPENGL_LIBRARIES}" )

set( TINYRENDERER_INCLUDES_DIRS
     "${GLFW_INCLUDE_DIRS}"
     "${GLEW_INCLUDE_DIRS}"
     "${CMAKE_CURRENT_SOURCE_DIR}/include"
     "${CMAKE_CURRENT_SOURCE_DIR}/ext/stb"
     "${CMAKE_SOURCE_DIR}/ext/tinymath/include"
     "${CMAKE_SOURCE_DIR}/ext/imgui"
     "${CMAKE_SOURCE_DIR}/ext/imgui/examples"
     "${CMAKE_SOURCE_DIR}/ext/spdlog/include" )
include_directories( ${TINYRENDERER_INCLUDES_DIRS} )

add_definitions( -DENGINE_RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/")
add_definitions( -DENGINE_EXAMPLES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/examples/" )

if ( TINYRENDERER_BUILD_WITH_LOGS OR ( CMAKE_BUILD_TYPE STREQUAL Debug ) )
    set( ENGINE_USE_LOGS "YES" )
    add_definitions( -DENGINE_USE_LOGS )
else()
    set( ENGINE_USE_LOGS "NO" )
endif()

file( GLOB_RECURSE SRCS "src/*.cpp" )

add_library( tinyrenderer_cpp_lib SHARED ${SRCS} )
target_link_libraries( tinyrenderer_cpp_lib
                       ${GLFW_LIBRARIES}
                       ${OPENGL_LIBRARIES}
                       ${GLEW_LIBRARIES}
                       tinymath_cpp_lib
                       assimp
                       spdlog
                       imgui )

set_property( TARGET tinyrenderer_cpp_lib PROPERTY TINYRENDERER_INCS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" )

if ( TINYRENDERER_BUILD_PYTHON_BINDINGS )
    add_subdirectory( bindings )
endif()

if ( TINYRENDERER_IS_MASTER_PROJECT AND TINYRENDERER_BUILD_DOCS )
    add_subdirectory( docs )
endif()

if ( TINYRENDERER_IS_MASTER_PROJECT AND TINYRENDERER_BUILD_EXAMPLES )
    add_subdirectory( examples )
endif()

message( "***************************************************************************************" )
message( "BUILD-OPTIONS SUMMARY:" )
message( "CMAKE_BUILD_TYPE                      : ${CMAKE_BUILD_TYPE}" )
message( "TINYRENDERER_BUILD_DOCS               : ${TINYRENDERER_BUILD_DOCS}" )
message( "TINYRENDERER_BUILD_EXAMPLES           : ${TINYRENDERER_BUILD_EXAMPLES}" )
message( "TNIYRENDERER_BUILD_PYTHON_BINDINGS    : ${TINYRENDERER_BUILD_PYTHON_BINDINGS}" )
message( "TINYRENDERER_BUILD_WITH_LOGS          : ${TINYRENDERER_BUILD_WITH_LOGS}" )
message( "ENGINE_USE_LOGS                       : ${ENGINE_USE_LOGS}" )
message( "***************************************************************************************" )