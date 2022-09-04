# ~~~
# CMake configuration for third-party dependencies.
#
# Dependencies:
# * pybind11
# * catch2
# * glfw
# * assimp
# * loco-utils
# * loco-math
#
# - Based on the superbuild script by jeffamstutz for ospray
#   https://github.com/jeffamstutz/superbuild_ospray/blob/main/macros.cmake
# - Based on MuJoCo's helper find_or_fetch content macro
#   https://github.com/deepmind/mujoco/blob/main/cmake/FindOrFetch.cmake
# ~~~

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
# Pybind11 is used for generating Python bindings for this project's C++ API
# ------------------------------------------------------------------------------
# Hint to Pybind11 that we want to use the newer FindPython module from CMake
set(PYBIND11_FINDPYTHON ON CACHE BOOL "Use newer FindPython (CMake 3.15+)")

loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE ${FIND_OR_FETCH_USE_SYSTEM_PACKAGE}
  PACKAGE_NAME pybind11
  LIBRARY_NAME pybind11
  GIT_REPO https://github.com/pybind/pybind11.git
  GIT_TAG v2.10.0
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
  GIT_TAG v2.x
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
  GIT_TAG 3.3.8
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
# Use Assimp from sources (again, might not be installed in the user's system).
# We use it for the wide support that it gives us to load models from various
# formats (e.g. GLTF, OBJ, etc.) and to avoid to re-write our own loaders
# ------------------------------------------------------------------------------
loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE ${FIND_OR_FETCH_USE_SYSTEM_PACKAGE}
  PACKAGE_NAME assimp
  LIBRARY_NAME assimp
  GIT_REPO https://github.com/assimp/assimp.git
  GIT_TAG master
  TARGETS assimp::assimp
  BUILD_ARGS
    -DBUILD_SHARED_LIBS=OFF
    -DASSIMP_BUILD_FRAMEWORK=OFF
    -DASSIMP_DOUBLE_PRECISION=OFF
    -DASSIMP_BUILD_ASSIMP_TOOLS=OFF
    -DASSIMP_BUILD_SAMPLES=OFF
    -DASSIMP_BUILD_TESTS=OFF
    -DASSIMP_INSTALL=OFF
    -DASSIMP_HUNTER_ENABLED=OFF
    -DASSIMP_INJECT_DEBUG_POSTFIX=OFF
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# LocoUtils exposes some utilities that we'll use (like logs, profiling, etc.)
# ------------------------------------------------------------------------------
loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  LIBRARY_NAME loco_utils
  GIT_REPO https://github.com/wpumacay/loco_utils.git
  GIT_TAG dev
  TARGETS loco::utils
  BUILD_ARGS
    -DLOCOUTILS_BUILD_PYTHON_BINDINGS=ON
    -DLOCOUTILS_BUILD_EXAMPLES=OFF
    -DLOCOUTILS_BUILD_DOCS=OFF
  EXCLUDE_FROM_ALL)

# ------------------------------------------------------------------------------
# LocoMath is used as math library (defines vectors, matrices, and operations
# that could be used on these types). The API is similar to Eigen's
# ------------------------------------------------------------------------------
loco_find_or_fetch_dependency(
  USE_SYSTEM_PACKAGE FALSE
  LIBRARY_NAME loco_math
  GIT_REPO https://github.com/wpumacay/loco_math.git
  GIT_TAG dev
  TARGETS loco::math loco::math_py_helpers
  BUILD_ARGS
    -DLOCOMATH_BUILD_PYTHON_BINDINGS=ON
    -DLOCOMATH_BUILD_EXAMPLES=OFF
    -DLOCOMATH_BUILD_TESTS=OFF
    -DLOCOMATH_BUILD_DOCS=OFF
  EXCLUDE_FROM_ALL)

# cmake-format: on
