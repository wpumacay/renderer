
cmake_minimum_required( VERSION 3.0.0 )

set( CMAKE_CXX_STANDARD 11 )

# compile everything as shared libraries
set( BUILD_SHARED_LIBS ON CACHE BOOL "Build libraries as shared" )

# compilation flags
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-format-security -Wno-sign-compare -Wno-unused-variable -g -fPIC " )

# outputs of the build should be sent accordingly
if ( NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY )
    message( "CAT1> Sending generated libs to OWN build directory" )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs" )
else()
    message( "CAT1> Sending generated libs to EXTERNAL build directory" )
endif()
message( "CAT1> LIBRARY_OUT_DIR: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" )

set( CAT1_IS_MASTER_PROJECT OFF )
if( CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( "CAT1> Configuring as MASTER project" )
    set( CAT1_IS_MASTER_PROJECT ON )
else()
    message( "CAT1> Configuring as CHILD project" )
endif()

# Add external dependencies only if master project, as if child project
if ( CAT1_IS_MASTER_PROJECT )
    add_subdirectory( ext )
endif()

message( "Finding required packages" )

find_package( OpenGL REQUIRED )
find_package( GLEW REQUIRED )
find_package( ASSIMP REQUIRED )
find_package( PkgConfig REQUIRED )
pkg_search_module( GLFW REQUIRED glfw3 )

##if ( APPLE )
    include_directories( /usr/local/include )
    link_directories( /usr/local/lib )
##endif()

message( "ASSIMP_INCLUDE_DIRS   : ${ASSIMP_INCLUDE_DIRS}" )
message( "GLFW_INCLUDE_DIRS     : ${GLFW_INCLUDE_DIRS}" )
message( "GLFW_LIBRARIES        : ${GLFW_LIBRARIES}" )
message( "GLFW_LIBRARY_DIRS     : ${GLFW_LIBRARY_DIRS}" )
message( "GLEW_INCLUDE_DIRS     : ${GLEW_INCLUDE_DIRS}" )
message( "GLEW_LIBRARIES        : ${GLEW_LIBRARIES}" )
message( "OPENGL_LIBRARIES      : ${OPENGL_LIBRARIES}" )

set( GLENGINE_INCLUDE_DIRS
     "${GLFW_INCLUDE_DIRS}"
     "${GLEW_INCLUDE_DIRS}"
     "${CMAKE_CURRENT_SOURCE_DIR}/include"
     "${CMAKE_CURRENT_SOURCE_DIR}/ext/stb"
     "${CMAKE_SOURCE_DIR}/ext/imgui"
     "${CMAKE_SOURCE_DIR}/ext/imgui/examples"
     "${CMAKE_SOURCE_DIR}/ext/spdlog/include" )

include_directories( ${GLENGINE_INCLUDE_DIRS} )

add_definitions( -DENGINE_RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/")
add_definitions( -DENGINE_EXAMPLES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/examples/" )

file( GLOB_RECURSE SRCS "src/*.cpp" )

add_library( cat1 SHARED ${SRCS} )

target_link_libraries( cat1 
                       ${GLFW_LIBRARIES} 
                       ${OPENGL_LIBRARIES} 
                       ${GLEW_LIBRARIES}
                       tinymath_cpp_lib
                       assimp
                       spdlog
                       imgui )

# whether or not to build examples
option( CAT1_BUILD_EXAMPLES "Set this ON to enabled building all examples" ON )

if ( CAT1_BUILD_EXAMPLES )
    # build some examples of the usage of our library
    add_subdirectory( examples )
endif()