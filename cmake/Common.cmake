# ~~~
# This file exposes some helper functions that are used in this project All
# functions and macros are "namespaced" with the prefix "tr_" for tiny_renderer
# ~~~

# Helper function that displays a message to the console, prefixed with the name
# of the project accordingly. Notice that we cam pass the log-level as optional
function(trMessage var_message)
  if(NOT PROJECT_NAME)
    return()
  endif()

  set(oneValueArgs LOG_LEVEL)
  cmake_parse_arguments(TM_PRINT "" "${oneValueArgs}" "" ${ARGN})
  if(TM_PRINT_LOG_LEVEL)
    message(${TM_PRINT_LOG_LEVEL} "${PROJECT_NAME} >>> ${var_message}")
  else()
    message(STATUS "${PROJECT_NAME} >>> ${var_message}")
  endif()
endfunction()

# Helper macro that initializes the project properly (whether if root or not)
function(trInitializeProject)
  if(NOT PROJECT_NAME)
    return()
  endif()

  # Grab the correct name for the variable to be exposed to the developer
  string(TOUPPER "${PROJECT_NAME}" ProjectNameUpper)
  string(TOUPPER "${PROJECT_NAME}_IS_ROOT_PROJECT" ProjectIsRootVarname)
  # @note: notice we're setting the variable twice (one for the local scope, and
  # another for the parent scope). This is required as we still use the variable
  # inside this function for some other checks (a bit of a hack >.<)
  if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(${ProjectIsRootVarname} TRUE)
    set(${ProjectIsRootVarname}
        TRUE
        PARENT_SCOPE)
    trMessage("The project ${PROJECT_NAME} is the root project")
  else()
    set(${ProjectIsRootVarname} FALSE)
    set(${ProjectIsRootVarname}
        FALSE
        PARENT_SCOPE)
    trMessage("The project ${PROJECT_NAME} is NOT the root project")
  endif()

  # Configure some project settings according to whether or not we are root
  if(${ProjectIsRootVarname})
    if(EXISTS "${PROJECT_SOURCE_DIR}/cmake")
      set(CMAKE_MODULE_PATH
          "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH}
          PARENT_SCOPE)
      trMessage("Added .cmake files for project ${PROJECT_NAME}")
    endif()
    # Configure the output-directory for SHARED libraries. For more information:
    # https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html, and
    # go to the section on library-output-artifacts for the details
    if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
      # If used as root project, then send all to this path
      set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
          "${CMAKE_BINARY_DIR}/libs"
          PARENT_SCOPE)
      trMessage("Sending generated SHARED libraries to OWN build directory")
    else()
      # If it's not the root project, then use the user-defined output
      # directory. This might be required for installation by the user
      trMessage("Sending generated SHARED libraries to USER-DEFINED directory")
    endif()
    # Configure the output-directory for STATIC libraries. For more information:
    # https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html, and
    # go to the section on archive-output-artifacts for the details
    if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
      # If used as root project, then send all to this path
      set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
          "${CMAKE_BINARY_DIR}/libs"
          PARENT_SCOPE)
      trMessage("Sending generated STATIC libraries to OWN build directory")
    else()
      # If it's not the root project, then use the user-defined output
      # directory. This might be required for installation by the user
      trMessage("Sending generated STATIC libraries to USER-DEFINED directory")
    endif()
    # Generate Position Independent Code (-fPIC), as bindings will need this.
    # Note that this is only set by the root-project (propagate from the root)
    if(NOT CMAKE_POSITION_INDEPENDENT_CODE)
      trMessage("Setting up -fPIC (Position Independent Code generation)")
      set(CMAKE_POSITION_INDEPENDENT_CODE
          ON
          PARENT_SCOPE)
    endif()
    # Export compile commands (used by clangd and other static analysis tools)
    if(NOT CMAKE_EXPORT_COMPILE_COMMANDS)
      trMessage("Generating compile commands for static analysis tools")
      set(CMAKE_EXPORT_COMPILE_COMMANDS
          ON
          PARENT_SCOPE)
    endif()
    # Make sure that if the user doesn't provide CMAKE_INSTALL_PREFIX, we then
    # use a default path for installation (relative to build). We only make sure
    # in the case we're the main project. If not, then the main project should
    # handle this
    if(NOT CMAKE_INSTALL_PREFIX)
      set(CMAKE_INSTALL_PREFIX
          "${CMAKE_BINARY_DIR}/install"
          PARENT_SCOPE)
      set(CMAKE_INSTALL_INCLUDEDIR
          "${CMAKE_INSTALL_PREFIX}/include"
          PARENT_SCOPE)
      set(CMAKE_INSTALL_LIBDIR
          "${CMAKE_INSTALL_PREFIX}/lib"
          PARENT_SCOPE)
      set(CMAKE_INSTALL_BINDIR
          "${CMAKE_INSTALL_PREFIX}/bin"
          PARENT_SCOPE)
      set(CMAKE_INSTALL_DOCDIR
          "${CMAKE_INSTALL_PREFIX}/doc"
          PARENT_SCOPE)
    endif()
  endif()

  # Expose some extra variables to the parent context
  set(PROJECT_NAME_UPPER
      ${ProjectNameUpper}
      PARENT_SCOPE)
  set(PROJECT_IS_ROOT_VARNAME
      ${ProjectIsRootVarname}
      PARENT_SCOPE)
endfunction()

# Helper macro that shows various settings obtained during configuration
function(trPrintSummary)
  if(NOT PROJECT_NAME)
    return()
  endif()
  # The list of all valid options exposed by the project. Notice that this part
  # is project specific, so we have to rewrite this macro on every project we
  # use (we could generalize it further, but for now this works ok)
  set(optionsArgs PYTHON_BINDINGS DOCS TESTS EXAMPLES LOGS PROFILING FFMPEG)
  # cmake-format: off
  message("|---------------------------------------------------------|")
  message("|                  TINY RENDERING ENGINE                  |")
  message("|---------------------------------------------------------|")
  message("
                        |\\      _,,,---,,_
                  ZZZzz /,`.-'`'    -.  ;-;;,_
                       |,4-  ) )-,_. ,\\ (  `'-'
                      '---''(_/--'  `-'\\_)" )
  # ASCII art from: https://www.asciiart.eu/animals/cats
  message("****************************************************")
  message("Build options summary for project ${PROJECT_NAME}")
  # General settings from cmake
  message("CMAKE_GENERATOR                : ${CMAKE_GENERATOR}")
  message("CMAKE_MODULE_PATH              : ${CMAKE_MODULE_PATH}")
  message("CMAKE_EXPORT_COMPILE_COMMANDS  : ${CMAKE_EXPORT_COMPILE_COMMANDS}")
  message("CMAKE_POSITION_INDEPENDENT_CODE: ${CMAKE_POSITION_INDEPENDENT_CODE}")
  message("CMAKE_C_COMPILER               : ${CMAKE_C_COMPILER}")
  message("CMAKE_CXX_COMPILER             : ${CMAKE_CXX_COMPILER}")
  message("CMAKE_BUILD_TYPE               : ${CMAKE_BUILD_TYPE}")
  message("CMAKE_LIBRARY_OUTPUT_DIRECTORY : ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
  message("CMAKE_ARCHIVE_OUTPUT_DIRECTORY : ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
  message("CMAKE_RUNTIME_OUTPUT_DIRECTORY : ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
  message("CMAKE_C_FLAGS                  : ${CMAKE_C_FLAGS}")
  message("CMAKE_CXX_FLAGS                : ${CMAKE_CXX_FLAGS}")
  message("CMAKE_INSTALL_PREFIX           : ${CMAKE_INSTALL_PREFIX}")
  message("CMAKE_INSTALL_INCLUDEDIR       : ${CMAKE_INSTALL_INCLUDEDIR}")
  message("CMAKE_INSTALL_LIBDIR           : ${CMAKE_INSTALL_LIBDIR}")
  message("CMAKE_INSTALL_DOCDIR           : ${CMAKE_INSTALL_DOCDIR}")
  message("CMAKE_INSTALL_BINDIR           : ${CMAKE_INSTALL_BINDIR}")
  message("CMAKE_PREFIX_PATH              : ${CMAKE_PREFIX_PATH}")
  message("CMAKE_BUILD_RPATH              : ${CMAKE_BUILD_RPATH}")
  # Some messages from the msvc compiler
  if(MSVC)
    message("MSVC_VERSION                   : ${MSVC_VERSION}")
  endif()

  # Project-specific settings
  message("${PROJECT_IS_ROOT_VARNAME} > ${${PROJECT_IS_ROOT_VARNAME}}")
  foreach(varOption IN LISTS optionsArgs)
    set(var_name "${PROJECT_NAME_UPPER}_BUILD_${varOption}")
    set(var_value "${${PROJECT_NAME_UPPER}_BUILD_${varOption}}")
    message("${var_name} > ${var_value}")
  endforeach()
  message("****************************************************")
  # cmake-format: on
endfunction()
