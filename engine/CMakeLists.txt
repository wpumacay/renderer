
message( "making the cat1-engine project" )

include_directories( include/ )

add_library( cat1gl_obj OBJECT
             src/app/LBaseApp.cpp
             src/window/LWindow.cpp
             src/camera/LICamera.cpp
             src/camera/LFixedCamera3d.cpp
             src/gl/LIndexBuffer.cpp
             src/gl/LVertexBuffer.cpp
             src/gl/LVertexArray.cpp
             src/gl/LTexture.cpp
             src/gl/LCubeTexture.cpp
             src/graphics/LSkybox.cpp
             src/graphics/LMesh.cpp
             src/graphics/LMeshBuilder.cpp
             src/material/LMaterial.cpp
             src/shader/LShaderManager.cpp
             src/resource/LAssetsManager.cpp
             #src/component
             #src/entity
             #src/input
             #src/light
             #src/renderer
             #src/scene
              )

add_library( cat1gl $<TARGET_OBJECTS:cat1gl_obj> )