
message( "making the cat1-engine project" )

include_directories( include/ )

add_library( cat1gl_obj OBJECT
             src/core/LBaseApp.cpp )

add_library( cat1gl $<TARGET_OBJECTS:cat1gl_obj> )