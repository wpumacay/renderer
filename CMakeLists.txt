
cmake_minimum_required( VERSION 3.0.0 )

set( CMAKE_CXX_STANDARD 11 )

message( "Configuring external dependencies" )

add_subdirectory( ext/glfw )
add_subdirectory( ext/glad )
add_subdirectory( ext/glm )

message( "DONE configuring external dependencies" )

message( "Configuring gl engine ..." )

add_definitions( -DENGINE_RESOURCES_PATH="${CMAKE_SOURCE_DIR}/res/")
message( "Current resources path: ${CMAKE_SOURCE_DIR}/res/" )

set( CAT1_INCLUDE_DIRECTORIES
     ext/glfw/include
	 ext/glad/include
     ext/glm
     ext/stb
	 include/ )

include_directories( ${CAT1_INCLUDE_DIRECTORIES} )

add_library( gengine
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
             src/debug/LDebugDrawer.cpp
             src/debug/LDebugSystem.cpp
             src/shaders/LShader.cpp
             src/shaders/LShaderManager.cpp
             src/LAssetsManager.cpp
             src/LVertexArray.cpp
             src/LVertexBuffer.cpp
             src/LWindow.cpp )

target_link_libraries( gengine 
                       glfw glad
                       ${OPENGL_LIBRARIES} )


message( "Configuring gl engine - done" )


message( "Configuring library tests ..." )


add_executable( test1
                tests/test1.cpp )
target_link_libraries( test1
                       gengine )



message( "Configuring library tests - done" )
