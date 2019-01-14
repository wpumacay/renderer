
message( "Configuring cat1::examples" )

include_directories( "../include" )

# Single test builder function
function( FcnBuildExample paramSources paramExecutableName )

    add_executable( ${paramExecutableName}
                    ${paramSources} )

    target_link_libraries( ${paramExecutableName}
                           cat1 )

endfunction()

file( GLOB SampleFiles *.cpp )

foreach( sampleFile ${SampleFiles} )
    # remove extension from sample file
    string( REPLACE ".cpp" "" executableLongName ${sampleFile} )
    # remove other path components from file
    get_filename_component( executableName ${executableLongName} NAME )

    FcnBuildExample( ${sampleFile} ${executableName} )

endforeach( sampleFile )