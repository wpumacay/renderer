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
# Define our project :)
project(
  LocoRenderer
  VERSION 0.0.1
  DESCRIPTION "A minimal renderer for prototyping 3D applications"
  HOMEPAGE_URL "https://github.com/wpumacay/loco_renderer"
  LANGUAGES C CXX)

# -------------------------------------
# Some set of options the user can play with
option(LOCORENDERER_BUILD_LOGS "Build with logs enabled" ON)
option(LOCORENDERER_BUILD_PROFILING "Build with profiling tools enabled" OFF)
option(LOCORENDERER_BUILD_PYTHON_BINDINGS "Build Python bindings" OFF)
option(LOCORENDERER_BUILD_EXAMPLES "Build C++ examples" OFF)
option(LOCORENDERER_BUILD_TESTS "Build C++ unit-tests" OFF)
option(LOCORENDERER_BUILD_DOCS "Build documentation" OFF)

# -------------------------------------
# Initialize the project
loco_initialize_project()

# -------------------------------------
# Get the required third-party dependencies
add_subdirectory(third_party)

find_package(OpenGL REQUIRED)

# cmake-format: off
# -------------------------------------
# Setup the main C++ target (LocoRendererCpp)
loco_create_target(
  LocoRendererCpp SHARED
  SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/source/loco/renderer/common.cpp
  INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/include
  TARGET_DEPENDENCIES
    OpenGL::OpenGL glfw::glfw assimp::assimp loco::math loco::utils
  WARNINGS_AS_ERRORS
    FALSE)
# cmake-format: on

# Expose an alias for the library (CMake namespace convention)
add_library(loco::renderer ALIAS LocoRendererCpp)

# -------------------------------------
# Show the properties of our main target
loco_print_target_properties(LocoRendererCpp)

# -------------------------------------
# Show some info of the this project
loco_print_project_info()
