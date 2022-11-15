include_guard()

if(NOT RENDERER_IS_ROOT_PROJECT)
  # Only build docs if we're the main project
  return()
endif()

if(NOT TARGET renderer::renderer)
  return()
endif()

# cmake-format: off
# -------------------------------------
# Initialize the documentation (doxygen + sphinx + breathe)
loco_setup_cppdocs(RendererCpp
  DOXYGEN_FILE_IN ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in
  DOXYGEN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/docs/Doxygen
  DOXYGEN_GENERATE_HTML FALSE
  DOXYGEN_GENERATE_LATEX FALSE
  DOXYGEN_GENERATE_XML TRUE
  DOXYGEN_QUIET TRUE
  SPHINX_FILE_IN ${PROJECT_SOURCE_DIR}/docs/conf.py.in
  SPHINX_OUTPUT_DIR ${CMAKE_BINARY_DIR}/docs/Sphinx
  SPHINX_BREATHE_PROJECT RendererCpp
  SPHINX_DOXYGEN_XML_OUTDIR ${CMAKE_BINARY_DIR}/docs/Doxygen/xml)
# cmake-format: on
