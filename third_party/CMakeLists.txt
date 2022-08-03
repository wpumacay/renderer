# ~~~
# CMake configuration for third-party dependencies.
#
# Dependencies:
# * pybind11
# * catch2
# * glfw
# * assimp
# * tiny-utils
# * tiny-math
#
# Based on the superbuild script by jeffamstutz for ospray
# https://github.com/jeffamstutz/superbuild_ospray/blob/main/macros.cmake
# ~~~

if(NOT LOCORENDERER_IS_ROOT_PROJECT)
  return()
endif()

# cmake-format: off

# ------------------------------------------------------------------------------
# Spdlog is used for the logging functionality (internally uses the fmt lib)
# ------------------------------------------------------------------------------
loco_configure_git_dependency(
    TARGET spdlog
    REPO https://github.com/gabime/spdlog.git
    TAG v1.9.2
    BUILD_MODE ${CMAKE_BUILD_TYPE}
    BUILD_ARGS
      -DSPDLOG_BUILD_SHARED=OFF
      -DSPDLOG_BUILD_EXAMPLE=OFF
      -DSPDLOG_BUILD_EXAMPLE_HO=OFF
      -DSPDLOG_BUILD_TESTS=OFF
      -DSPDLOG_BUILD_TESTS_HO=OFF
      -DSPDLOG_BUILD_BENCH=OFF)

if(NOT TARGET spdlog::spdlog)
  loco_message("We require [spdlog] as a dependency, but couldn't be retrieved
    from their repo" LOG_LEVEL FATAL_ERROR)
endif()
# ------------------------------------------------------------------------------
# Pybind11 is used for generating Python bindings for this project's C++ API.
# Notice that we're using a forked version in which usage of unique-ptr is
# allowed, as we use these over shared-ptrs in most parts of the codebase
# ------------------------------------------------------------------------------
loco_configure_git_dependency(
  TARGET pybind11
  REPO https://github.com/RobotLocomotion/pybind11.git
  TAG drake
  BUILD_MODE RelWithDebInfo
  BUILD_ARGS
    -DPYBIND11_TEST=OFF)

# ------------------------------------------------------------------------------
# Catch2 is used for making unit-tests in C++ land. It's API is simple yet quite
# powerfull (e.g. we can make use of template-parametrized tests-cases)
# ------------------------------------------------------------------------------
loco_configure_git_dependency(
  TARGET catch2
  REPO https://github.com/catchorg/Catch2.git
  TAG v2.x
  BUILD_MODE RelWithDebInfo
    -DCATCH_INSTALL_DOCS=OFF
    -DCATCH_INSTALL_EXTRAS=OFF
    -DCATCH_DEVELOPMENT_BUILD=OFF)

# ------------------------------------------------------------------------------
# Use GLFW from sources (as it might not be installed in the system). We use
# GLFW in order to create a window with a proper GL context in many platforms.
# Recall that this might not be the only backend for window creation (e.g. Qt)
# ------------------------------------------------------------------------------
loco_configure_git_dependency(
  TARGET glfw
  REPO https://github.com/glfw/glfw.git
  TAG 3.3.8
  BUILD_MODE RelWithDebInfo
  BUILD_ARGS
    -DGLFW_BUILD_EXAMPLES=OFF
    -DGLFW_BUILD_TESTS=OFF
    -DGLFW_BUILD_DOCS=OFF
    -DGLFW_INSTALL=OFF)
# Make an alias (sorry, kind of an OCD thingy :/)
add_library(glfw::glfw ALIAS glfw)

# TODO(wilbert): add support to handle more backends (e.g. EGL, MESA, Qt, etc.)

# ------------------------------------------------------------------------------
# Use Assimp from sources (again, might not be installed in the user's system).
# We use it for the wide support that it gives us to load models from various
# formats (e.g. GLTF, OBJ, etc.) and to avoid to re-write our own loaders
# ------------------------------------------------------------------------------
loco_configure_git_dependency(
  TARGET assimp
  REPO https://github.com/assimp/assimp.git
  TAG v5.0.1
  BUILD_MODE RelWithDebInfo
  BUILD_ARGS
    -DBUILD_SHARED_LIBS=OFF
    -DASSIMP_BUILD_FRAMEWORK=OFF
    -DASSIMP_DOUBLE_PRECISION=OFF
    -DASSIMP_BUILD_ASSIMP_TOOLS=OFF
    -DASSIMP_BUILD_SAMPLES=OFF
    -DASSIMP_BUILD_TESTS=OFF
    -DASSIMP_INSTALL=OFF
    -DASSIMP_HUNTER_ENABLED=OFF
    -DASSIMP_INJECT_DEBUG_POSTFIX=OFF)

# ------------------------------------------------------------------------------
# LocoUtils exposes some utilities that we'll use (like logs, profiling, etc.)
# ------------------------------------------------------------------------------
loco_configure_git_dependency(
  TARGET LocoUtils
  REPO https://github.com/wpumacay/loco_utils.git
  TAG dev
  BUILD_MODE ${CMAKE_BUILD_TYPE}
  BUILD_ARGS
    -DLOCOUTILS_BUILD_PYTHON_BINDINGS=ON
    -DLOCOUTILS_BUILD_EXAMPLES=OFF
    -DLOCOUTILS_BUILD_DOCS=OFF
  DEPENDS_ON
    pybind11 spdlog)

# ------------------------------------------------------------------------------
# LocoMath is used as math library (defines vectors, matrices, and operations
# that could be used on these types). The API is similar to Eigen's
# ------------------------------------------------------------------------------
loco_configure_git_dependency(
  TARGET LOcoMath
  REPO https://github.com/wpumacay/loco_math.git
  TAG dev
  BUILD_MODE ${CMAKE_BUILD_TYPE}
  BUILD_ARGS
    -DTINYMATH_BUILD_PYTHON_BINDINGS=ON
    -DTINYMATH_BUILD_EXAMPLES=OFF
    -DTINYMATH_BUILD_TESTS=OFF
    -DTINYMATH_BUILD_DOCS=OFF
  DEPENDS_ON
    pybind11)

# cmake-format: on
