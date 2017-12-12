
message( "generating tutorials" )

include_directories( ../engine/include )
link_directories( ../lib )

if( WIN32 )

    add_executable( tut_texture.exe tut_texture.cpp )
    target_link_libraries( tut_texture glew32s glfw3 opengl32 )

elseif( UNIX AND NOT APPLE )

    add_executable( tut_texture tut_texture.cpp )
    target_link_libraries( tut_texture 
                           cat1gl 
                           glfw 
                           #glewd
                           GLEW::GLEW
                           ${OPENGL_LIBRARIES} )

    add_executable( tut_skybox tut_skybox.cpp )
    target_link_libraries( tut_skybox 
                           cat1gl 
                           glfw 
                           #glewd
                           GLEW::GLEW
                           ${OPENGL_LIBRARIES} )

elseif( APPLE )

    add_executable( tut_texture tut_texture.cpp )
    target_link_libraries( tut_texture 
                           cat1gl 
                           glfw 
                           glewd
                           ${OPENGL_LIBRARIES} )

endif()