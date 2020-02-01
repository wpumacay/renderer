message( "ENGINE> Configuring samples: HEADLESS" )

file( GLOB SampleFilesHeadless *.cpp )

foreach( sampleFile ${SampleFilesHeadless} )
    # remove extension from sample file
    string( REPLACE ".cpp" "" executableLongName ${sampleFile} )
    # remove other path components from file
    get_filename_component( executableName ${executableLongName} NAME )

    FcnBuildExample( ${sampleFile} ${executableName} )

endforeach( sampleFile )