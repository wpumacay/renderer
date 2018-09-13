
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

add_library( gengine SHARED
             src/LCommon.cpp
             src/LFixedCamera3d.cpp
             src/LFpsCamera.cpp
             src/LInputHandler.cpp
             src/LICamera.cpp
             src/LIndexBuffer.cpp
             src/LTexture.cpp
             src/LCubeTexture.cpp
             src/LSkybox.cpp
             src/LFrameBuffer.cpp
             src/LShadowMap.cpp
             src/LMeshBuilder.cpp
             src/LMesh.cpp
             src/LScene.cpp
             src/LMeshRenderer.cpp
             src/LMasterRenderer.cpp
             src/debug/LDebugDrawer.cpp
             src/debug/LDebugSystem.cpp
             src/shaders/LShader.cpp
             src/shaders/LShaderManager.cpp
             src/LAssetsManager.cpp
             src/LVertexArray.cpp
             src/LVertexBuffer.cpp
             src/LWindow.cpp
             src/LApp.cpp )

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
