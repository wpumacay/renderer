
message( "generating tests project" )

if( WIN32 )

	add_executable( test_glfw
					test_glfw.cpp )
	target_link_libraries( test_glfw glew32s glfw3 opengl32 )

elseif( UNIX )

	add_executable( test_glfw.out
					test_glfw.cpp )
	target_link_libraries( test_glfw.out 
                           ${OPENGL_LIBRARIES} 
                           ${GLFW_LIBRARIES} 
                           GLEW::GLEW 
                           ${X11_LIBRARIES} 
                           ${CMAKE_THREAD_LIBS_INIT}
                           dl )

elseif( APPLE )

    # TODO: Define executable properties here

endif()