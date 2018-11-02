
cmake_minimum_required( VERSION 3.0.0 )

set( CMAKE_CXX_STANDARD 11 )

message( "Configuring external dependencies" )

find_package( OpenGL REQUIRED )
find_package( GLEW REQUIRED )
find_package( PkgConfig REQUIRED )
pkg_search_module( GLFW REQUIRED glfw3 )

# message( "GLFW_INCLUDE_DIRS: ${GLFW_INCLUDE_DIRS}" )
# message( "GLFW_LIBRARIES: ${GLFW_LIBRARIES}" )
# message( "GLFW_LIBRARY_DIRS: ${GLFW_LIBRARY_DIRS}" )

message( "GLEW_INCLUDE_DIRS: ${GLEW_INCLUDE_DIRS}" )
message( "GLEW_LIBRARIES: ${GLEW_LIBRARIES}" )

include_directories( ${GLFW_INCLUDE_DIRS} )
include_directories( ${GLEW_INCLUDE_DIRS} )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/include" )
include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/ext/stb" )

add_definitions( -DENGINE_RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/")

file( GLOB_RECURSE SRCS "src/*.cpp" )

if( BUILD_SHARED_LIBS )
    message( "Building engine as SHARED library" )
    set( CMAKE_CXX_FLAGS "-fPIC ${CMAKE_CXX_FLAGS}" )
    add_library( gengine SHARED
                ${SRCS} )
else()
    message( "Building engine as STATIC library" )
    add_library( gengine
                ${SRCS} )
endif()

target_link_libraries( gengine 
                       ${GLFW_LIBRARIES} 
                       ${OPENGL_LIBRARIES} 
                       ${GLEW_LIBRARIES} )

add_executable( test1
                tests/test1.cpp )
target_link_libraries( test1
                       gengine )

add_executable( test2
                tests/test2.cpp )
target_link_libraries( test2
                       gengine )
