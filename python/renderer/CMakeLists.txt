if(NOT TARGET renderer::renderer)
  return()
endif()

set(BINDINGS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/window_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/keycodes_py.cpp)

pybind11_add_module(renderer MODULE ${BINDINGS_SOURCES})
target_link_libraries(renderer PUBLIC renderer::renderer math::math_py_helpers)
