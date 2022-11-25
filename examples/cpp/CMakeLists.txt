if(NOT TARGET renderer::renderer)
  return()
endif()

set(RENDERER_EXAMPLES_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/example_01_window.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/example_02_shaders.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/example_03_buffers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/example_04_buffers_with_indices.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/example_05_textures.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/example_06_managers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/example_07_camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/example_08_geometry.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/example_09_geometry_factory.cpp)

foreach(example_filepath IN LISTS RENDERER_EXAMPLES_LIST)
  loco_setup_single_file_example(${example_filepath} TARGET_DEPENDENCIES
                                 renderer::renderer)
endforeach()
