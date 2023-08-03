include_guard()

# ~~~
# CMake configuration for third-party dependencies.
#
# Dependencies:
# * pybind11
# * catch2
# * glfw
# * assimp
# * utils
# * math
#
# - Based on the superbuild script by jeffamstutz for ospray
#   https://github.com/jeffamstutz/superbuild_ospray/blob/main/macros.cmake
# - Based on MuJoCo's helper find_or_fetch content macro
#   https://github.com/deepmind/mujoco/blob/main/cmake/FindOrFetch.cmake
# ~~~

# -------------------------------------
set(RENDERER_DEP_VERSION_catch2
    3f0283de7a9c43200033da996ff9093be3ac84dc # Release 3.3.2
    CACHE STRING "Version of Catch2 to be fetched (used for unittests)")

set(RENDERER_DEP_VERSION_glfw
    7482de6071d21db77a7236155da44c172a7f6c9e # Release 3.3.8
    CACHE STRING
          "Version of GLFW to be fetched (used to create an xplatf. window) ")

set(RENDERER_DEP_VERSION_imgui
    458a1090314a965dd37b02c918d83077a0142ad5 # Release 1.89.3
    CACHE STRING
          "Version of Dear-ImGui to be fetched (used for prototyping UI)")

set(RENDERER_DEP_VERSION_utils
    58623f20534cbe4cc3f20cdeac46a27ca4f21790 # Version v0.2.2
    CACHE STRING "Version of internal utilities repo to be fetched")

set(RENDERER_DEP_VERSION_math
    7f863e0dbf9f0e9ba0d35b38ab912baf7431053f # Version v0.5.1
    CACHE STRING "Version of internal math repo to be fetched")

set(RENDERER_DEP_VERSION_pybind11
    5b0a6fc2017fcc176545afe3e09c9f9885283242 # Release v2.10.4
    CACHE STRING "Version of PyBind11 to be fetched (used for python bindings)")

mark_as_advanced(RENDERER_DEP_VERSION_catch2)
mark_as_advanced(RENDERER_DEP_VERSION_glfw)
mark_as_advanced(RENDERER_DEP_VERSION_imgui)
mark_as_advanced(RENDERER_DEP_VERSION_utils)
mark_as_advanced(RENDERER_DEP_VERSION_math)
mark_as_advanced(RENDERER_DEP_VERSION_pybind11)

# -------------------------------------
find_package(OpenGL REQUIRED)

if(OpenGL_EGL_FOUND)
  target_link_libraries(OpenGL::OpenGL INTERFACE OpenGL::EGL)
endif()

# -------------------------------------
option(FIND_OR_FETCH_USE_SYSTEM_PACKAGE
       "Whether or not to give priority to system-wide package search" OFF)

# cmake-format: off
# ------------------------------------------------------------------------------
# Pybind11 is used for generating Python bindings for this project's C++ API.
# ------------------------------------------------------------------------------

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE ${FIND_OR_FETCH_USE_SYSTEM_PACKAGE}
  PACKAGE_NAME pybind11
  LIBRARY_NAME pybind11
  GIT_REPO https://github.com/pybind/pybind11.git
  GIT_TAG ${RENDERER_DEP_VERSION_pybind11}
  GIT_PROGRESS FALSE
  TARGETS pybind11::headers
  BUILD_ARGS
    -DPYBIND11_TEST=OFF
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# Catch2 is used for making unit-tests in C++ land. It's API is simple yet quite
# powerfull (e.g. we can make use of template-parametrized tests-cases)
# ------------------------------------------------------------------------------
loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE ${FIND_OR_FETCH_USE_SYSTEM_PACKAGE}
  PACKAGE_NAME Catch2
  LIBRARY_NAME catch2
  GIT_REPO https://github.com/catchorg/Catch2.git
  GIT_TAG ${RENDERER_DEP_VERSION_catch2}
  GIT_PROGRESS FALSE
  TARGETS Catch2::Catch2
  BUILD_ARGS
    -DCATCH_INSTALL_DOCS=OFF
    -DCATCH_INSTALL_EXTRAS=OFF
    -DCATCH_DEVELOPMENT_BUILD=OFF
  EXCLUDE_FROM_ALL)

# Add custom scripts for test-case registration to the module path
if (catch2_POPULATED)
  list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/contrib")
endif()

# ------------------------------------------------------------------------------
# Use GLFW from sources (as it might not be installed in the system). We use
# GLFW in order to create a window with a proper GL context in many platforms.
# Recall that this might not be the only backend for window creation (e.g. Qt)
# ------------------------------------------------------------------------------
loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE ${FIND_OR_FETCH_USE_SYSTEM_PACKAGE}
  PACKAGE_NAME glfw3
  LIBRARY_NAME glfw3
  GIT_REPO https://github.com/glfw/glfw.git
  GIT_TAG ${RENDERER_DEP_VERSION_glfw}
  GIT_PROGRESS FALSE
  TARGETS glfw
  BUILD_ARGS
    -DGLFW_BUILD_EXAMPLES=OFF
    -DGLFW_BUILD_TESTS=OFF
    -DGLFW_BUILD_DOCS=OFF
    -DGLFW_INSTALL=OFF
  EXCLUDE_FROM_ALL)
# Make an alias (sorry, kind of an OCD thingy xD)
add_library(glfw::glfw ALIAS glfw)

# ------------------------------------------------------------------------------
# ImGui is used for fast prototyping Graphical User Interfaces in Immediate mode
# ------------------------------------------------------------------------------

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG ${RENDERER_DEP_VERSION_imgui}
  GIT_PROGRESS FALSE
  GIT_SHALLOW FALSE
  USES_TERMINAL_DOWNLOAD TRUE)
FetchContent_GetProperties(imgui)

if (NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
endif()


# ------------------------------------------------------------------------------
# "Utils" exposes some utilities that we'll use (like logs, profiling, etc.)
# ------------------------------------------------------------------------------
loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  LIBRARY_NAME utils
  GIT_REPO https://github.com/wpumacay/utils.git
  GIT_TAG ${RENDERER_DEP_VERSION_utils}
  GIT_PROGRESS FALSE
  TARGETS utils::utils
  BUILD_ARGS
    -DUTILS_BUILD_PYTHON_BINDINGS=ON
    -DUTILS_BUILD_EXAMPLES=OFF
    -DUTILS_BUILD_DOCS=OFF
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# "Math" is used as math library (defines vectors, matrices, and operations
# that could be used on these types). The API is similar to Eigen's
# ------------------------------------------------------------------------------
loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  LIBRARY_NAME math
  GIT_REPO https://github.com/wpumacay/math.git
  GIT_TAG ${RENDERER_DEP_VERSION_math}
  GIT_PROGRESS FALSE
  TARGETS math::math math::math_py_helpers
  BUILD_ARGS
    -DMATH_BUILD_PYTHON_BINDINGS=ON
    -DMATH_BUILD_EXAMPLES=OFF
    -DMATH_BUILD_TESTS=OFF
    -DMATH_BUILD_DOCS=OFF
  EXCLUDE_FROM_ALL)

# cmake-format: on
