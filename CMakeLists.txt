
cmake_minimum_required( VERSION 3.0.0 )

set( CMAKE_CXX_STANDARD 11 )

message( "Configuring external dependencies" )

find_package( OpenGL REQUIRED )
find_package( GLEW REQUIRED )
find_package( assimp REQUIRED )
find_package( PkgConfig REQUIRED )
pkg_search_module( GLFW REQUIRED glfw3 )

message( "ASSIMP_INCLUDE_DIRS: ${ASSIMP_INCLUDE_DIRS}" )

# message( "GLFW_INCLUDE_DIRS: ${GLFW_INCLUDE_DIRS}" )
# message( "GLFW_LIBRARIES: ${GLFW_LIBRARIES}" )
# message( "GLFW_LIBRARY_DIRS: ${GLFW_LIBRARY_DIRS}" )

message( "GLEW_INCLUDE_DIRS: ${GLEW_INCLUDE_DIRS}" )
message( "GLEW_LIBRARIES: ${GLEW_LIBRARIES}" )

set( GLENGINE_INCLUDE_DIRS
     "${GLFW_INCLUDE_DIRS}"
     "${GLEW_INCLUDE_DIRS}"
     "${CMAKE_CURRENT_SOURCE_DIR}/include"
     "${CMAKE_CURRENT_SOURCE_DIR}/ext/stb" )

include_directories( ${GLENGINE_INCLUDE_DIRS} )

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-format-security -Wno-sign-compare -Wno-unused-variable -g" )

add_definitions( -DENGINE_RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/")

file( GLOB_RECURSE SRCS "src/*.cpp" )

message( "Building engine as SHARED library" )
set( CMAKE_CXX_FLAGS "-fPIC ${CMAKE_CXX_FLAGS}" )
add_library( cat1 SHARED
             ${SRCS} )

target_link_libraries( cat1 
                       ${GLFW_LIBRARIES} 
                       ${OPENGL_LIBRARIES} 
                       ${GLEW_LIBRARIES}
                       assimp )

add_subdirectory( examples )