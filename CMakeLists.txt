cmake_minimum_required(VERSION 3.15..3.19)

# -------------------------------------
# Get loco_cmake to help us configure our CMake based project
include(FetchContent)
FetchContent_Declare(
  loco_cmake
  GIT_REPOSITORY https://github.com/wpumacay/loco_cmake.git
  GIT_TAG dev
  GIT_PROGRESS TRUE
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(loco_cmake)
include(${loco_cmake_SOURCE_DIR}/Index.cmake)
# -------------------------------------

project(
  LocoRenderer
  VERSION 0.2.2
  DESCRIPTION "A minimal renderer for prototyping 3D applications"
  HOMEPAGE_URL "https://github.com/wpumacay/loco_renderer"
  LANGUAGES C CXX)

option(LOCORENDERER_BUILD_IMGUI "Build with support 4 Ocornut's Dear ImGui" ON)
option(LOCORENDERER_BUILD_LOGS "Build with logs enabled" ON)
option(LOCORENDERER_BUILD_PROFILING "Build with profiling tools enabled" OFF)
option(LOCORENDERER_BUILD_PYTHON_BINDINGS "Build Python bindings" ON)
option(LOCORENDERER_BUILD_EXAMPLES "Build C++ examples" ON)
option(LOCORENDERER_BUILD_TESTS "Build C++ unit-tests" OFF)
option(LOCORENDERER_BUILD_DOCS "Build documentation" OFF)

loco_initialize_project()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(LocoRendererDependencies)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/source/renderer)

# Include extra third-party dependencies
add_subdirectory(third_party)

# cmake-format: off
# -------------------------------------
loco_create_target(LocoRendererCpp STATIC
  SOURCES
    ${SOURCE_DIR}/window/window_t.cpp
    ${SOURCE_DIR}/window/window_properties.cpp
    ${SOURCE_DIR}/window/impl/window_impl_glfw.cpp
    ${SOURCE_DIR}/window/impl/window_impl_egl.cpp
    ${SOURCE_DIR}/shader/shader_t.cpp
    ${SOURCE_DIR}/shader/program_t.cpp
    ${SOURCE_DIR}/core/vertex_buffer_layout_t.cpp
    ${SOURCE_DIR}/core/vertex_buffer_t.cpp
    ${SOURCE_DIR}/core/vertex_array_t.cpp
    ${SOURCE_DIR}/core/index_buffer_t.cpp
    ${SOURCE_DIR}/core/texture_data_t.cpp
    ${SOURCE_DIR}/core/texture_t.cpp
    ${SOURCE_DIR}/assets/shader_manager_t.cpp
    ${SOURCE_DIR}/assets/texture_manager_t.cpp
  INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  TARGET_DEPENDENCIES
    OpenGL::OpenGL glfw::glfw loco::math loco::utils stb glad
  WARNINGS_AS_ERRORS
    FALSE)

if (LOCORENDERER_BUILD_IMGUI)
  target_sources(LocoRendererCpp PRIVATE
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
  target_include_directories(LocoRendererCpp PUBLIC ${imgui_SOURCE_DIR})
  target_compile_definitions(LocoRendererCpp PUBLIC -DLOCORENDERER_IMGUI)
endif()
# cmake-format: on

include(LocoRendererDefinitions)

add_library(renderer::renderer ALIAS LocoRendererCpp)

if(LOCORENDERER_BUILD_EXAMPLES)
  add_subdirectory(examples/cpp)
endif()

if(LOCORENDERER_BUILD_PYTHON_BINDINGS)
  add_subdirectory(python/renderer)
endif()

if(LOCORENDERER_BUILD_DOCS)
  include(LocoRendererDocs)
endif()

loco_print_target_properties(LocoRendererCpp)

loco_print_project_info()
