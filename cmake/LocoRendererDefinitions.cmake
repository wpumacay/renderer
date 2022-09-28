include_guard()

# -------------------------------------
# Configure resources path appropriately
if(NOT DEFINED LOCORENDERER_RESOURCES_PATH)
  target_compile_definitions(
    LocoRendererCpp
    PUBLIC -DLOCORENDERER_RESOURCES_PATH="${PROJECT_SOURCE_DIR}/resources/")
else()
  target_compile_definitions(
    LocoRendererCpp
    PUBLIC -DLOCORENDERER_RESOURCES_PATH="${LOCORENDERER_RESOURCES_PATH}")
endif()

# -------------------------------------
# Configure examples resource folder path appropriately
if(NOT DEFINED LOCORENDERER_EXAMPLES_PATH)
  target_compile_definitions(
    LocoRendererCpp
    PUBLIC -DLOCORENDERER_EXAMPLES_PATH="${PROJECT_SOURCE_DIR}/examples/")
else()
  target_compile_definitions(
    LocoRendererCpp
    PUBLIC -DLOCORENDERER_EXAMPLES_PATH="${LOCORENDERER_EXAMPLES_PATH}")
endif()
