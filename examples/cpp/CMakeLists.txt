message( "TINYRENDERER::examples::cpp >>> configuring C/C++ examples" )

include_directories( "${TINYRENDERER_INCLUDES_DIRS}" )

# Define test builder helper for windowed examples
function( FcnBuildExampleWindowed paramSources paramExecutableName )
    add_executable( ${paramExecutableName}
                    ${paramSources} )
    target_link_libraries( ${paramExecutableName}
                           tinyrenderer_cpp_lib_glfw
                           tinymath_cpp_lib )
endfunction()

# Define test builder helper for headless examples
function( FcnBuildExampleHeadless paramSources paramExecutableName )
    add_executable( ${paramExecutableName}
                    ${paramSources} )
    target_link_libraries( ${paramExecutableName}
                           tinyrenderer_cpp_lib_egl
                           tinymath_cpp_lib )
endfunction()

if ( TINYRENDERER_BUILD_WINDOWED_GLFW )
add_subdirectory( core )
#### add_subdirectory( lights )
#### add_subdirectory( shadows )
#### add_subdirectory( fog )
#### add_subdirectory( math )
#### add_subdirectory( instancing )
#### add_subdirectory( renderer )
#### add_subdirectory( picking )
#### add_subdirectory( application )
endif()

if ( TINYRENDERER_BUILD_HEADLESS_EGL )
add_subdirectory( headless )
endif()