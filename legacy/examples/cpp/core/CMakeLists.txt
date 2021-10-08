message("ENGINE> Configuring samples: CORE")

file(GLOB SampleFilesCore *.cpp)
set(SampleFilesCore # "sample_buffers.cpp"
    # "sample_texture.cpp"
    "sample_window.cpp" "sample_shaders.cpp")

foreach(sampleFile ${SampleFilesCore})
  # remove extension from sample file
  string(REPLACE ".cpp" "" executableLongName ${sampleFile})
  # remove other path components from file
  get_filename_component(executableName ${executableLongName} NAME)

  FcnBuildExample(${sampleFile} ${executableName})

endforeach(sampleFile)
