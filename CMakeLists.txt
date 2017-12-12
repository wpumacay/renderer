
cmake_minimum_required( VERSION 3.5.0 )
project( cat1 C CXX )

set( CMAKE_BUILD_TYPE Debug )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS_DEBUG "-g" )
set( CMAKE_CXX_FLAGS_RELEASE "-O3" )

if( WIN32 )

	message( "GENERATING SOLUTIONS FOR WINDOWS" )

	find_package( OpenGL REQUIRED )
	include_directories( engine )


elseif( UNIX AND NOT APPLE )

	message( "GENERATING MAKEFILES/PROJECTS FOR LINUX" )

	find_package( OpenGL REQUIRED )
    find_package( GLEW REQUIRED )
	include_directories( engine )

elseif( APPLE )

	message( "GENERATING MAKEFILES/PROJECTS FOR MACOS" )

	find_package( OpenGL REQUIRED )
	include_directories( engine )

endif()

#add_subdirectory( ext/glew )
add_subdirectory( ext/glfw )
add_subdirectory( ext/glm )

include_directories( ext/stb_image )
#include_directories( ext/glew/include )
include_directories( ext/glfw/include )
include_directories( ext/glm/ )

add_subdirectory( engine )
add_subdirectory( tutorials )




