cmake_minimum_required(VERSION 3.15..3.19)

project(
  TinyRenderer
  VERSION 0.1.0
  DESCRIPTION "A minimal renderer for prototyping 3D applications"
  HOMEPAGE_URL "https://github.com/wpumacay/tiny_renderer"
  LANGUAGES CXX)

# Include some helper functions that we will need later
include(${PROJECT_SOURCE_DIR}/cmake/Common.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/Utils.cmake)

# Include some internal tools we might need to use
include(FetchContent)
include(GNUInstallDirs)

# Check if we're currently working as the root project. If so, configure some
# common settings that we might need later during configuration
trInitializeProject()

option(TINYRENDERER_BUILD_LOGS "Build with logs enabled" ON)
option(TINYRENDERER_BUILD_FFMPEG "Build with FFMPEG support" OFF)
option(TINYRENDERER_BUILD_PROFILING "Build with profiling tools enabled" OFF)
option(TINYRENDERER_BUILD_PYTHON_BINDINGS "Build Python bindings" ON)
option(TINYRENDERER_BUILD_DOCS "Build documentation" OFF)
option(TINYRENDERER_BUILD_TESTS "Build C++ unit-tests" OFF)
option(TINYRENDERER_BUILD_EXAMPLES "Build C++ examples" ON)

# Find required packages
find_package(OpenGL REQUIRED)
# find_package(Qt5 REQUIRED)

# Add third-party dependencies (mostly git-repos)
add_subdirectory(third_party)

add_library(
  TinyRendererCpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/CCommon.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/CMath.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core/CIndexBuffer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core/CVertexBuffer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core/CVertexBufferLayout.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core/CVertexArray.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/CShader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/CProgram.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/CShaderManager.cpp)
target_link_libraries(
  TinyRendererCpp PUBLIC OpenGL::OpenGL glfw::glfw assimp::assimp
                         tinymath::tinymath tinyutils::tinyutils)

# @todo(wilbert): setup specific dependencies accordingly. This might include
# setting up conditional dependencies for EGL, GLFW, OSMesa, Qt, etc.

add_library(tinyrenderer::tinyrenderer ALIAS TinyRendererCpp)

trPrintSummary()
