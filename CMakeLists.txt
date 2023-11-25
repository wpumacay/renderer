cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# -------------------------------------
# Get loco_cmake to help us configure our CMake based project
include(FetchContent)
FetchContent_Declare(
  loco_cmake
  GIT_REPOSITORY https://github.com/wpumacay/loco_cmake.git
  GIT_TAG d1ba4734dcf29bbc7318deea16213452a80b3f2a # Version v0.1.4
  GIT_PROGRESS FALSE
  GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(loco_cmake)
include(${loco_cmake_SOURCE_DIR}/Index.cmake)
# -------------------------------------

project(
  Renderer
  VERSION 0.3.8
  DESCRIPTION "A minimal renderer for prototyping 3D applications"
  HOMEPAGE_URL "https://github.com/wpumacay/renderer"
  LANGUAGES C CXX)

option(RENDERER_BUILD_IMGUI "Build with support 4 Ocornut's Dear ImGui" ON)
option(RENDERER_BUILD_LOGS "Build with logs enabled" ON)
option(RENDERER_BUILD_PROFILING "Build with profiling tools enabled" OFF)
option(RENDERER_BUILD_PYTHON_BINDINGS "Build Python bindings" ON)
option(RENDERER_BUILD_EXAMPLES "Build C++ examples" ON)
option(RENDERER_BUILD_TESTS "Build C++ unit-tests" ON)
option(RENDERER_BUILD_DOCS "Build documentation" OFF)

# cmake-format: off
set(RENDERER_BUILD_CXX_STANDARD 17 CACHE STRING "The C++ standard to be used")
set_property(CACHE RENDERER_BUILD_CXX_STANDARD PROPERTY STRINGS 11 14 17 20)
# cmake-format: on

loco_initialize_project(CXX_STANDARD ${RENDERER_BUILD_CXX_STANDARD})

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(RendererDependencies)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/source/renderer)

# Include extra third-party dependencies
add_subdirectory(third_party)

# cmake-format: off
# -------------------------------------
loco_create_target(RendererCpp STATIC
  SOURCES
    ${SOURCE_DIR}/window/window_t.cpp
    ${SOURCE_DIR}/window/window_config_t.cpp
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
    ${SOURCE_DIR}/camera/camera_t.cpp
    ${SOURCE_DIR}/camera/camera_controller_t.cpp
    ${SOURCE_DIR}/camera/orbit_camera_controller_t.cpp
    ${SOURCE_DIR}/camera/fps_camera_controller_t.cpp
    ${SOURCE_DIR}/input/input_manager_t.cpp
    ${SOURCE_DIR}/geometry/geometry_t.cpp
    ${SOURCE_DIR}/geometry/geometry_factory.cpp
    ${SOURCE_DIR}/light/light_t.cpp
    ${SOURCE_DIR}/material/material_t.cpp
    ${SOURCE_DIR}/debug/debug_drawer_t.cpp
    ${SOURCE_DIR}/application_t.cpp
  INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  TARGET_DEPENDENCIES
    OpenGL::OpenGL glfw::glfw math::math utils::utils stb glad
  CXX_STANDARD
    ${RENDERER_BUILD_CXX_STANDARD}
  WARNINGS_AS_ERRORS
    FALSE)

if (RENDERER_BUILD_IMGUI)
  target_sources(RendererCpp PRIVATE
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp)
  target_include_directories(RendererCpp PUBLIC ${imgui_SOURCE_DIR})
  target_compile_definitions(RendererCpp PUBLIC -DRENDERER_IMGUI)
endif()
# cmake-format: on

include(RendererDefinitions)

add_library(renderer::renderer ALIAS RendererCpp)

if(RENDERER_BUILD_EXAMPLES)
  add_subdirectory(examples/cpp)
endif()

if(RENDERER_BUILD_TESTS)
  add_subdirectory(tests/cpp)
endif()

if(RENDERER_BUILD_PYTHON_BINDINGS)
  add_subdirectory(python/renderer/bindings)
endif()

# -------------------------------------
# Build the required documentation accordingly
if(RENDERER_BUILD_DOCS)
  # cmake-format: off
  loco_setup_cppdocs(RendererCpp
    DOXYGEN_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in
    DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs/Doxygen
    DOXYGEN_GENERATE_HTML FALSE
    DOXYGEN_GENERATE_LATEX FALSE
    DOXYGEN_GENERATE_XML TRUE
    DOXYGEN_QUIET TRUE
    SPHINX_FILE_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/conf.py.in
    SPHINX_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs/Sphinx
    SPHINX_BREATHE_PROJECT RendererCpp
    SPHINX_DOXYGEN_XML_OUTDIR ${CMAKE_CURRENT_BINARY_DIR}/docs/Doxygen/xml)
  # cmake-format: on
endif()

loco_print_target_properties(RendererCpp)

loco_print_project_info()
