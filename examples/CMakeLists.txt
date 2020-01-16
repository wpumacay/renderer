
message( "Configuring tinyrenderer::examples" )

include_directories( "../include" )

# Single test builder function
function( FcnBuildExample paramSources paramExecutableName )

    add_executable( ${paramExecutableName}
                    ${paramSources} )

    target_link_libraries( ${paramExecutableName}
                           tinyrenderer_cpp_lib
                           tinymath_cpp_lib )

endfunction()

add_subdirectory( core )
add_subdirectory( lights )
add_subdirectory( shadows )
add_subdirectory( fog )
add_subdirectory( math )
add_subdirectory( instancing )
add_subdirectory( renderer )
add_subdirectory( picking )
add_subdirectory( application )