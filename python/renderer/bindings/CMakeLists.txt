if(NOT TARGET renderer::renderer)
  return()
endif()

# cmake-format: off
set(BINDINGS_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/window_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/keycodes_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/buttons_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shader_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/buffers_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/texture_py.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/managers_py.cpp
)
# cmake-format: on

pybind11_add_module(renderer_bindings MODULE ${BINDINGS_SOURCES})
target_link_libraries(renderer_bindings PUBLIC renderer::renderer
                                               math::math_py_helpers)
