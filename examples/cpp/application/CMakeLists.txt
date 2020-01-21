
message( "ENGINE> Configuring samples: APPLICATION" )

file( GLOB SampleFilesApplication *.cpp )

foreach( sampleFile ${SampleFilesApplication} )
    # remove extension from sample file
    string( REPLACE ".cpp" "" executableLongName ${sampleFile} )
    # remove other path components from file
    get_filename_component( executableName ${executableLongName} NAME )

    FcnBuildExample( ${sampleFile} ${executableName} )

endforeach( sampleFile )