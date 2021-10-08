# ~~~
# This file exposes some utilities that are used throughout this project
# ~~~

# Helper macro that sets up a required git dependency for this project
#
# Based on the superbuild script by jeffamstutz for ospray
# https://github.com/jeffamstutz/superbuild_ospray/blob/main/macros.cmake
macro(trConfigureGitDependency)
  set(oneValArgs TARGET REPO TAG BUILD_MODE DISCARD_UNLESS)
  set(multiValArgs BUILD_ARGS)
  cmake_parse_arguments(GIT_DEP "" "${oneValArgs}" "${multiValArgs}" ${ARGN})
  # Check if the user passed the DISCARD_UNLESS argument (if not, set to TRUE)
  if(NOT DEFINED GIT_DEP_DISCARD_UNLESS)
    set(GIT_DEP_DISCARD_UNLESS TRUE)
  endif()
  # In case the user discard the dep. then stop (might not be required)
  if(${GIT_DEP_DISCARD_UNLESS})
    # cmake-format: off
    # cmake-lint: disable=C0103
    set(FETCHCONTENT_QUIET FALSE CACHE INTERNAL "Show git-progress" FORCE)
    FetchContent_Declare(
          ${GIT_DEP_TARGET}
          GIT_REPOSITORY ${GIT_DEP_REPO}
          GIT_TAG ${GIT_DEP_TAG}
          GIT_PROGRESS TRUE
          GIT_SHALLOW TRUE
          USES_TERMINAL_DOWNLOAD TRUE
          PREFIX "${CMAKE_SOURCE_DIR}/third_party/${GIT_DEP_TARGET}"
          DOWNLOAD_DIR "${CMAKE_SOURCE_DIR}/third_party/${GIT_DEP_TARGET}"
          SOURCE_DIR "${CMAKE_SOURCE_DIR}/third_party/${GIT_DEP_TARGET}/source"
          BINARY_DIR "${CMAKE_BINARY_DIR}/third_party/${GIT_DEP_TARGET}/build"
          STAMP_DIR "${CMAKE_BINARY_DIR}/third_party/${GIT_DEP_TARGET}/stamp"
          TMP_DIR "${CMAKE_BINARY_DIR}/third_party/${GIT_DEP_TARGET}/tmp"
          CMAKE_ARGS -DCMAKE_BUILD_TYPE=${GIT_DEP_BUILD_MODE}
                     -DCMAKE_GENERATOR=${CMAKE_GENERATOR}
                     -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                     -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                     -DCMAKE_INSTALL_INCLUDEDIR=${CMAKE_INSTALL_INCLUDEDIR}
                     -DCMAKE_INSTALL_LIBDIR=${CMAKE_INSTALL_LIBDIR}
                     -DCMAKE_INSTALL_DOCDIR=${CMAKE_INSTALL_DOCDIR}
                     -DCMAKE_INSTALL_BINDIR=${CMAKE_INSTALL_BINDIR}
                     -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
                     ${GIT_DEP_BUILD_ARGS}
          BUILD_ALWAYS OFF)
    # cmake-format: on
    FetchContent_MakeAvailable(${GIT_DEP_TARGET})
  endif()
endmacro()

# Helper macro that configures the target for a given example path
macro(trConfigureExample example_filepath)
  # Make sure that the main library was built
  if(NOT TARGET tinyrenderer::tinyrenderer)
    trMessage(
      "Must have target tinyrenderer::tinyrenderer for configuring an example"
      LOG_LEVEL WARNING)
    return()
  endif()
  # Get the name of the target for the given example
  get_filename_component(ex_target_name ${example_filepath} NAME_WLE)
  # Create the target for this example
  add_executable(${ex_target_name} ${example_filepath})
  target_link_libraries(${ex_target_name} PRIVATE tinyrenderer::tinyrenderer)
endmacro()
