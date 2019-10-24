
message( "Configuring cat1::examples" )

include_directories( "../include" )

# Single test builder function
function( FcnBuildExample paramSources paramExecutableName )

    add_executable( ${paramExecutableName}
                    ${paramSources} )

    target_link_libraries( ${paramExecutableName}
                           cat1 )

endfunction()

add_subdirectory( core )
add_subdirectory( lights )
add_subdirectory( shadows )
# add_subdirectory( fog )
# add_subdirectory( math )
# add_subdirectory( instancing )
# add_subdirectory( renderer )
# add_subdirectory( picking )
add_subdirectory( application )