# cmake-format: off
loco_create_target(stb INTERFACE
  SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.h
    ${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image_write.h
  INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR})

loco_create_target(glad INTERFACE
  SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/gl.c
    ${CMAKE_CURRENT_SOURCE_DIR}/glad/egl.c
  INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/glad)

# cmake-format: on
