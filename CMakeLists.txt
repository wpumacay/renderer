
cmake_minimum_required( VERSION 3.5.0 )
project( cat1 )

if( WIN32 )

	message( "making cat1 project for windows" )

	include_directories( engine 
						 ext/glew/include 
						 ext/glfw/include 
						 glm/ )
	
	link_directories( ext/glew/lib/Release/x64 
					  ext/glfw/lib-vc2015 )

elseif( UNIX )

	find_package( OpenGL REQUIRED )
	find_package( GLEW REQUIRED )
	find_package( PkgConfig REQUIRED )
	pkg_search_module( GLFW REQUIRED glfw3 )

	include_directories( engine )

	link_directories( usr/lib usr/lib64 usr/local/lib )

elseif( APPLE )

	# TODO: Define project generation for xcode

endif()


add_subdirectory( engine )
add_subdirectory( tests )




