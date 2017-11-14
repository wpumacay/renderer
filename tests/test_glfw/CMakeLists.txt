
#cmake_minimum_required( VERSION 3.5.0 )
#project( cat1_tests )

message( "making tests" )

if( WIN32 )

	add_executable( test_glfw
					test_glfw.cpp )
	target_link_libraries( test_glfw glew32s glfw3 opengl32 )

elseif( UNIX )

	add_executable( test_glfw.out
					test_glfw.cpp )
	target_link_libraries( test_glfw.out ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} GLEW::GLEW )

endif()