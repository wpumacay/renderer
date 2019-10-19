
message( "ENGINE> Configuring samples: INSTANCING" )

file( GLOB SampleFilesInstancing *.cpp )

foreach( sampleFile ${SampleFilesInstancing} )
    # remove extension from sample file
    string( REPLACE ".cpp" "" executableLongName ${sampleFile} )
    # remove other path components from file
    get_filename_component( executableName ${executableLongName} NAME )

    FcnBuildExample( ${sampleFile} ${executableName} )

endforeach( sampleFile )