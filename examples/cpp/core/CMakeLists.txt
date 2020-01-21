
message( "ENGINE> Configuring samples: CORE" )

file( GLOB SampleFilesCore *.cpp )

foreach( sampleFile ${SampleFilesCore} )
    # remove extension from sample file
    string( REPLACE ".cpp" "" executableLongName ${sampleFile} )
    # remove other path components from file
    get_filename_component( executableName ${executableLongName} NAME )

    FcnBuildExample( ${sampleFile} ${executableName} )

endforeach( sampleFile )