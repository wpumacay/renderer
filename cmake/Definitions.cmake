include_guard()

# -------------------------------------
# Configure resources path appropriately
if(NOT DEFINED RENDERER_RESOURCES_PATH)
  target_compile_definitions(
    RendererCpp
    PUBLIC -DRENDERER_RESOURCES_PATH="${PROJECT_SOURCE_DIR}/resources/")
else()
  target_compile_definitions(
    RendererCpp PUBLIC -DRENDERER_RESOURCES_PATH="${RENDERER_RESOURCES_PATH}")
endif()

# -------------------------------------
# Configure examples resource folder path appropriately
if(NOT DEFINED RENDERER_EXAMPLES_PATH)
  target_compile_definitions(
    RendererCpp
    PUBLIC -DRENDERER_EXAMPLES_PATH="${PROJECT_SOURCE_DIR}/examples/")
else()
  target_compile_definitions(
    RendererCpp PUBLIC -DRENDERER_EXAMPLES_PATH="${RENDERER_EXAMPLES_PATH}")
endif()
