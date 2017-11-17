
message( "generating test_gl project" )

include_directories( ../../engine/include )

if( WIN32 )

    add_executable( test_gl_ch2_1 test_gl_ch2_1.cpp )
    target_link_libraries( test_gl_ch2_1 glew32s glfw3 opengl32 )

elseif( UNIX )

    add_executable( test_gl_ch2_1.out test_gl_ch2_1.cpp )
    target_link_libraries( test_gl_ch2_1.out cat1gl
                           ${OPENGL_LIBRARIES} 
                           ${GLFW_LIBRARIES} 
                           GLEW::GLEW 
                           ${X11_LIBRARIES} 
                           ${CMAKE_THREAD_LIBS_INIT}
                           dl )

elseif( APPLE )

    # TODO: Define executable properties here

endif()