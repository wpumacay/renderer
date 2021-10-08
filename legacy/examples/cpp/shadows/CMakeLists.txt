message("ENGINE> Configuring samples: SHADOWS")

file(GLOB SampleFilesShadows *.cpp)

foreach(sampleFile ${SampleFilesShadows})
  # remove extension from sample file
  string(REPLACE ".cpp" "" executableLongName ${sampleFile})
  # remove other path components from file
  get_filename_component(executableName ${executableLongName} NAME)

  FcnBuildExampleWindowed(${sampleFile} ${executableName})

endforeach(sampleFile)
