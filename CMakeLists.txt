
message( "Configuring gl engine ..." )

include_directories( ${TYSOC_INCLUDE_DIRECTORIES} )

add_library( gengine
             src/LCommon.cpp
             src/LComponent.cpp
             src/LEntity.cpp
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
             src/LMeshComponent.cpp
             src/LHeightmapGenerator.cpp
             src/LTerrainPatch.cpp
             src/LMesh.cpp
             src/LScene.cpp
             src/LTerrainGenerator.cpp
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
