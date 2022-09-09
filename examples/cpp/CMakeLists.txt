if(NOT TARGET loco::renderer)
  return()
endif()

set(LOCORENDERER_EXAMPLES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/example_01_window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/example_02_shaders.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/example_03_buffers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/example_04_buffers_with_indices.cpp)

foreach(example_filepath IN LISTS LOCORENDERER_EXAMPLES_LIST)
  loco_setup_single_file_example(${example_filepath} TARGET_DEPENDENCIES
                                 loco::renderer)
endforeach()
