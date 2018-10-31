
cmake_minimum_required( VERSION 3.0.0 )

set( CMAKE_CXX_STANDARD 11 )

message( "Configuring external dependencies" )

option( CAT1_BUILD_EXT_LIBS "Build external dependencies along with the project" ON )

if( CAT1_BUILD_EXT_LIBS )
    message( "BUILDING EXTERNAL DEPENDENCIES" )

    # Avoid building the examples in the libraries
    set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs" )
    set( GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs" )
    set( GLM_TEST_ENABLE OFF CACHE BOOL "Build unit tests" )

	add_subdirectory( ext/glfw )
	add_subdirectory( ext/glad )
    add_subdirectory( ext/glm )

    set( CAT1_INCLUDE_DIRECTORIES
         ext/glfw/include
         ext/glad/include
         ext/glm )

    add_definitions( -DENGINE_RESOURCES_PATH="${CMAKE_SOURCE_DIR}/res/")
    message( "Current resources path: ${CMAKE_SOURCE_DIR}/res/" )

else()
    message( "USING PREBUILT EXTERNAL DEPENDENCIES" )
    if ( NOT CAT1_INCLUDE_DIRECTORIES )
        message( "WARNING: SHOULD HAVE SET THE VARIABLE CAT1_INCLUDE_DIRECTORIES FOR EXTERNAL DEPENDENCIES" )
        set( CAT1_INCLUDE_DIRECTORIES
             "${CMAKE_SOURCE_DIR}/ext/glfw/include"
             "${CMAKE_SOURCE_DIR}/ext/glad/include"
             "${CMAKE_SOURCE_DIR}/ext/glm/" )
    endif()

    if ( NOT CAT1_ENGINE_RESOURCES_PATH )
        message( "WARNING: SHOULD HAVE SET THE VARIABLE CAT1_ENGINE_RESOURCES_PATH FOR THE RESOURCES FOLDER" )
        set( CAT1_ENGINE_RESOURCES_PATH
             "${CMAKE_SOURCE_DIR}/ext/cat1/res/" )
    endif()

    add_definitions( -DENGINE_RESOURCES_PATH="${CAT1_ENGINE_RESOURCES_PATH}" )
    message( "Current resources path: ${CAT1_ENGINE_RESOURCES_PATH}" )
endif()

message( "DONE configuring external dependencies" )

message( "Configuring gl engine ..." )

set( CAT1_INCLUDE_DIRECTORIES "${CAT1_INCLUDE_DIRECTORIES};ext/stb;include/" )
message( "include-dirs : ${CAT1_INCLUDE_DIRECTORIES}" )
include_directories( ${CAT1_INCLUDE_DIRECTORIES} )

file( GLOB_RECURSE SRCS "src/*.cpp" )

if( BUILD_SHARED_LIBS )
    message( "Building engine as SHARED library" )
    set( CMAKE_CXX_FLAGS "-fPIC ${CMAKE_CXX_FLAGS}" )
    message( "Flags: ${CMAKE_CXX_FLAGS}" )
    add_library( gengine SHARED
                ${SRCS} )
else()
    message( "Building engine as STATIC library" )
    add_library( gengine
                ${SRCS} )
endif()

target_link_libraries( gengine 
                       glfw glad
                       ${OPENGL_LIBRARIES} )


message( "Configuring gl engine - done" )


message( "Configuring library tests ..." )


add_executable( test1
                tests/test1.cpp )
target_link_libraries( test1
                       gengine )

add_executable( test2
                tests/test2.cpp )
target_link_libraries( test2
                       gengine )

message( "Configuring library tests - done" )
