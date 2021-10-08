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

# cmake-format: off

# ------------------------------------------------------------------------------
# Pybind11 is used for generating Python bindings for this project's C++ API.
# Notice that we're using a forked version in which usage of unique-ptr is
# allowed, as we use these over shared-ptrs in most parts of the codebase
# ------------------------------------------------------------------------------
trConfigureGitDependency(
    TARGET pybind11
    REPO https://github.com/RobotLocomotion/pybind11.git
    TAG drake
    BUILD_MODE Release
    BUILD_ARGS
      -DPYBIND11_TEST=OFF
    DISCARD_UNLESS TINYRENDERER_BUILD_PYTHON_BINDINGS)

# ------------------------------------------------------------------------------
# Catch2 is used for making unit-tests in C++ land. It's API is simple yet quite
# powerfull (e.g. we can make use of template-parametrized tests-cases)
# ------------------------------------------------------------------------------
trConfigureGitDependency(
    TARGET catch2
    REPO https://github.com/catchorg/Catch2.git
    TAG v2.13.7
    BUILD_MODE Release
    BUILD_ARGS
      -DCATCH_INSTALL_DOCS=OFF
      -DCATCH_INSTALL_EXTRAS=OFF
      -DCATCH_DEVELOPMENT_BUILD=OFF
    DISCARD_UNLESS TINYRENDERER_BUILD_TESTS)

# ------------------------------------------------------------------------------
# Use GLFW from sources (as it might not be installed in the system). We use
# GLFW in order to create a window with a proper GL context in many platforms.
# Recall that this might not be the only backend for window creation (e.g. Qt)
# ------------------------------------------------------------------------------
trConfigureGitDependency(
    TARGET glfw
    REPO https://github.com/glfw/glfw.git
    TAG 3.3.4
    BUILD_MODE Release
    BUILD_ARGS
      -DGLFW_BUILD_EXAMPLES=OFF
      -DGLFW_BUILD_TESTS=OFF
      -DGLFW_BUILD_DOCS=OFF
      -DGLFW_INSTALL=OFF)
# Make an alias (sorry, kind of an OCD thingy :/)
add_library(glfw::glfw ALIAS glfw)

# @todo(wilbert): add support to handle more backends (e.g. EGL, MESA, Qt, etc.)

# ------------------------------------------------------------------------------
# Use Assimp from sources (again, might not be installed in the user's system).
# We use it for the wide support that it gives us to load models from various
# formats (e.g. GLTF, OBJ, etc.) and to avoid to re-write our own loaders
# ------------------------------------------------------------------------------
trConfigureGitDependency(
    TARGET assimp
    REPO https://github.com/assimp/assimp.git
    TAG v5.0.1
    BUILD_MODE Release
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
# TinyUtils exposes some utilities that we'll use (like logs, profiling, etc.)
# ------------------------------------------------------------------------------
trConfigureGitDependency(
    TARGET tinyutils
    REPO https://github.com/wpumacay/tiny_utils.git
    TAG v0.1.1
    BUILD_MODE ${CMAKE_BUILD_TYPE}
    BUILD_ARGS
      -DTINYUTILS_BUILD_PYTHON_BINDINGS=ON
      -DTINYUTILS_BUILD_EXAMPLES=OFF
      -DTINYUTILS_BUILD_DOCS=OFF)
# @todo(wilbert): remove alias once we add it on the main repo
add_library(tinyutils::tinyutils ALIAS TinyUtilsCpp)

# ------------------------------------------------------------------------------
# TinyMath is used as math library (defines vectors, matrices, and operations
# that could be used on these types). The API is similar to Eigen's
# ------------------------------------------------------------------------------
trConfigureGitDependency(
    TARGET tinymath
    REPO https://github.com/wpumacay/tiny_math.git
    TAG v0.0.0
    BUILD_MODE ${CMAKE_BUILD_TYPE}
    BUILD_ARGS
      -DTINYMATH_BUILD_PYTHON_BINDINGS=ON
      -DTINYMATH_BUILD_EXAMPLES=OFF
      -DTINYMATH_BUILD_TESTS=OFF
      -DTINYMATH_BUILD_DOCS=OFF)
# @todo(wilbert): remove alias once we're using the refactored version
add_library(tinymath::tinymath ALIAS tinymath_cpp_lib)

# cmake-format: on
