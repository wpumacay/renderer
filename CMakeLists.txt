
cmake_minimum_required( VERSION 3.5.0 )
project( cat1 C CXX )

set( CMAKE_BUILD_TYPE Debug )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS_DEBUG "-g -Wno-unused-parameter" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3 -Wno-unused-parameter" )

if( WIN32 )

	message( "making cat1 project for windows" )

	include_directories( engine 
						 ext/glew/include 
						 ext/glfw/include 
						 glm/ )
	
	link_directories( ext/glew/lib/Release/x64 
					  ext/glfw/lib-vc2015 )

elseif( UNIX )

	message( "making cat1 project for linux" )

	find_package( OpenGL REQUIRED )
	find_package( GLUT REQUIRED )
	find_package( GLEW REQUIRED )
	find_package( X11 REQUIRED )
	find_package( Threads REQUIRED )
	find_package( PkgConfig REQUIRED )
	pkg_search_module( GLFW REQUIRED glfw3 )

	include_directories( engine ${GLFW_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS} )

elseif( APPLE )

	message( "for mac, just glut support for the moment" )

	find_package( OpenGL REQUIRED )
	find_package( GLUT REQUIRED )
	find_package( GLEW REQUIRED )

endif()


add_subdirectory( engine )
add_subdirectory( tests )




