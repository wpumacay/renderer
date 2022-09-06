if(NOT TARGET loco::renderer)
  return()
endif()

set(BINDINGS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/window_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/keycodes_py.cpp)

pybind11_add_module(renderer MODULE ${BINDINGS_SOURCES})
target_link_libraries(renderer PUBLIC loco::renderer loco::math_py_helpers)
