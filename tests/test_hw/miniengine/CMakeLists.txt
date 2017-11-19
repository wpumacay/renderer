message( "making the mini-engine library" )

include_directories( include/ ../../../engine/include )

add_library( miniengine_obj OBJECT
             src/LCamera3D.cpp
             src/LMesh.cpp
             src/LMeshBuilder.cpp
             src/LScene.cpp
             src/LShader.cpp
             src/LShaderManager.cpp )

add_library( miniengine $<TARGET_OBJECTS:miniengine_obj> )