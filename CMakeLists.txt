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
  VERSION 0.2.1
  DESCRIPTION "A minimal renderer for prototyping 3D applications"
  HOMEPAGE_URL "https://github.com/wpumacay/loco_renderer"
  LANGUAGES C CXX)

option(LOCORENDERER_BUILD_LOGS "Build with logs enabled" ON)
option(LOCORENDERER_BUILD_PROFILING "Build with profiling tools enabled" OFF)
option(LOCORENDERER_BUILD_PYTHON_BINDINGS "Build Python bindings" ON)
option(LOCORENDERER_BUILD_EXAMPLES "Build C++ examples" ON)
option(LOCORENDERER_BUILD_TESTS "Build C++ unit-tests" OFF)
option(LOCORENDERER_BUILD_DOCS "Build documentation" ON)

loco_initialize_project()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(LocoRendererDependencies)

set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/source/loco/renderer)

# Include extra third-party dependencies
add_subdirectory(third_party)

# cmake-format: off
# -------------------------------------
loco_create_target(LocoRendererCpp STATIC
  SOURCES
    ${SOURCE_DIR}/common.cpp
    ${SOURCE_DIR}/window/window.cpp
    ${SOURCE_DIR}/window/window_properties.cpp
    ${SOURCE_DIR}/window/impl/window_impl_glfw.cpp
    ${SOURCE_DIR}/window/impl/window_impl_egl.cpp
    ${SOURCE_DIR}/shader/shader_t.cpp
    ${SOURCE_DIR}/shader/program_t.cpp
  INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  TARGET_DEPENDENCIES
    OpenGL::OpenGL glfw::glfw assimp::assimp loco::math loco::utils stb glad
  WARNINGS_AS_ERRORS
    FALSE)
# cmake-format: on

add_library(loco::renderer ALIAS LocoRendererCpp)

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
