
message( "generating test_gl project" )

add_subdirectory( miniengine )

include_directories( ../../engine/include miniengine/include/ )

if( WIN32 )

    add_executable( hw1 hw1.cpp )
    target_link_libraries( hw1 glew32s glfw3 opengl32 )

elseif( UNIX )

    add_executable( hw1_2.out hw1_2.cpp )
    target_link_libraries( hw1_2.out cat1gl miniengine
                           ${OPENGL_LIBRARIES} 
                           ${GLFW_LIBRARIES} 
                           GLEW::GLEW 
                           ${X11_LIBRARIES} 
                           ${CMAKE_THREAD_LIBS_INIT}
                           dl
                           GLU
                           ${GLUT_LIBRARY} )

elseif( APPLE )

    # TODO: Define executable properties here

endif()