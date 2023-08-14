# ~~~
# CMake configuration for C++ unittests
# ~~~
if(NOT TARGET renderer::renderer)
  loco_message("Unittests require target [renderer::renderer], but wasn't found"
               LOG_LEVEL WARNING)
  return()
endif()

if(NOT TARGET Catch2::Catch2)
  loco_message("Unittests require target [Catch2::Catch2], but wasn't found"
               LOG_LEVEL WARNING)
  return()
endif()

include(CTest)
include(Catch)

add_executable(
  RendererCppTests
  ${CMAKE_CURRENT_SOURCE_DIR}/test_main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/test_window_config.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/test_window.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/test_shader.cpp)

target_link_libraries(RendererCppTests PRIVATE renderer::renderer
                                               Catch2::Catch2)
target_include_directories(RendererCppTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
catch_discover_tests(RendererCppTests)
