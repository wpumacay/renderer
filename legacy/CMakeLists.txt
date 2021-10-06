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

option(TINYRENDERER_BUILD_FFMPEG "Build with FFMPEG support" OFF)
option(TINYRENDERER_BUILD_LOGS "Build with logs enabled (requires spdlog)" ON)
option(TINYRENDERER_BUILD_PROFILING "Build with profiling tools enabled" OFF)
option(TINYRENDERER_BUILD_PYTHON_BINDINGS "Build python bindings" ON)
option(TINYRENDERER_BUILD_DOCS "Build documentation (requires Doxygen)" OFF)
option(TINYRENDERER_BUILD_TESTS "Build C++ unit-tests (requires Catch2)" OFF)
option(TINYRENDERER_BUILD_EXAMPLES "Build C++ examples" ON)

# Add third-party dependencies to the build (brings targets)
add_subdirectory(third_party)

find_package(ASSIMP REQUIRED)
find_package(OpenGL REQUIRED)

set(TINYRENDERER_INCLUDES_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/ext/stb"
    "${CMAKE_SOURCE_DIR}/ext/tiny_math/include"
    "${CMAKE_SOURCE_DIR}/ext/tiny_utils/include"
    "${CMAKE_SOURCE_DIR}/ext/imgui"
    "${CMAKE_SOURCE_DIR}/ext/imgui/examples"
    "${CMAKE_SOURCE_DIR}/ext/spdlog/include")
include_directories(${TINYRENDERER_INCLUDES_DIRS})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ext/glad")

set(TINYRENDERER_RESOURCES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/res/")
add_definitions(-DENGINE_RESOURCES_PATH="${TINYRENDERER_RESOURCES_PATH}")
add_definitions(
  -DENGINE_EXAMPLES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/examples/cpp/")

# CHECK FEATURE USAGE: LOGGING support
# ##############################################################################
if(TINYRENDERER_BUILD_LOGS)
  set(ENGINE_LOGS_ENABLED ON)
else()
  set(ENGINE_LOGS_ENABLED OFF)
endif()
# ##############################################################################

# CHECK FEATURE USAGE: PROFILING support
# ##############################################################################
if(TINYRENDERER_BUILD_PROFILING)
  set(ENGINE_PROFILING_ENABLED ON)
else()
  set(ENGINE_PROFILING_ENABLED OFF)
endif()
# ##############################################################################

# CHECK FEATURE USAGE: TRACK-ALLOCATIONS support
# ##############################################################################
if(TINYRENDERER_BUILD_WITH_TRACK_ALLOCS)
  set(ENGINE_TRACK_ALLOCS_ENABLED ON)
else()
  set(ENGINE_TRACK_ALLOCS_ENABLED OFF)
endif()
# ##############################################################################

# CHECK FEATURE AVAILABILITY: GLFW support
# ##############################################################################
message(
  "***************************************************************************************"
)
message("TINYRENDERER >>> Looking for GLFW support")
find_package(PkgConfig)
if(PKG_CONFIG_FOUND)
  pkg_search_module(GLFW glfw3)
  if(GLFW_FOUND)
    set(ENGINE_GLFW_ENABLED ON)
    message("TINYRENDERER >>> Found GLFW support")
  else()
    set(ENGINE_GLFW_ENABLED OFF)
    message("TINYRENDERER >>> Couldn't find GLFW support")
  endif()
else()
  set(ENGINE_GLFW_ENABLED OFF)
  message("TINYRENDERER >>> Couldn't find GLFW support")
endif()
message(
  "***************************************************************************************"
)
# ##############################################################################

# CHECK FEATURE AVAILABILITY: EGL support
# ##############################################################################
message(
  "***************************************************************************************"
)
message("TINYRENDERER >>> Looking for EGL support")
if(OpenGL_EGL_FOUND)
  set(ENGINE_EGL_ENABLED ON)
  message("TINYRENDERER >>> Found EGL support")
else()
  set(ENGINE_EGL_ENABLED OFF)
  message("TINYRENDERER >>> Couldn't find EGL support")
endif()
message(
  "***************************************************************************************"
)
# ##############################################################################

# CHECK FEATURE AVAILABILITY: FFMPEG support
# ##############################################################################
message(
  "***************************************************************************************"
)
message("TINYRENDERER >>> Looking for libav-libraries (to build ffmpeg utils)")
# Search libav libraries
find_path(AVCODEC_INCLUDE_DIRS libavcodec/avcodec.h)
find_path(AVFORMAT_INCLUDE_DIRS libavformat/avformat.h)
find_path(AVUTIL_INCLUDE_DIRS libavutil/avutil.h)
find_path(AVDEVICE_INCLUDE_DIRS libavdevice/avdevice.h)
find_path(SWRESAMPLE_INCLUDE_DIRS libswresample/swresample.h)
find_path(SWSCALE_INCLUDE_DIRS libswscale/swscale.h)

find_library(AVCODEC_LIB avcodec)
find_library(AVFORMAT_LIB avformat)
find_library(AVUTIL_LIB avutil)
find_library(AVDEVICE_LIB avdevice)
find_library(SWRESAMPLE_LIB swresample)
find_library(SWSCALE_LIB swscale)

message("AVCODEC_INCLUDE_DIRS   : ${AVCODEC_INCLUDE_DIRS}")
message("AVFORMAT_INCLUDE_DIRS  : ${AVFORMAT_INCLUDE_DIRS}")
message("AVUTIL_INCLUDE_DIRS    : ${AVUTIL_INCLUDE_DIRS}")
message("AVDEVICE_INCLUDE_DIRS  : ${AVDEVICE_INCLUDE_DIRS}")
message("SWRESAMPLE_INCLUDE_DIRS: ${SWRESAMPLE_INCLUDE_DIRS}")
message("SWSCALE_INCLUDE_DIRS   : ${SWSCALE_INCLUDE_DIRS}")

message("AVCODEC_LIB   : ${AVCODEC_LIB}")
message("AVFORMAT_LIB  : ${AVFORMAT_LIB}")
message("AVUTIL_LIB    : ${AVUTIL_LIB}")
message("AVDEVICE_LIB  : ${AVDEVICE_LIB}")
message("SWRESAMPLE_LIB: ${SWRESAMPLE_LIB}")
message("SWSCALE_LIB   : ${SWSCALE_LIB}")

if(AVCODEC_LIB STREQUAL "AVCODEC_LIB-NOTFOUND"
   OR AVFORMAT_LIB STREQUAL "AVFORMAT_LIB-NOTFOUND"
   OR AVUTIL_LIB STREQUAL "AVUTIL_LIB-NOTFOUND"
   OR AVDEVICE_LIB STREQUAL "AVDEVICE_LIB-NOTFOUND"
   OR SWRESAMPLE_LIB STREQUAL "SWRESAMPLE_LIB-NOTFOUND"
   OR SWSCALE_LIB STREQUAL "SWSCALE_LIB-NOTFOUND")
  set(FFMPEG_FOUND OFF) # @todo: check alternative using the old method
                        # (pkg-config + hard-coded /usr/lib/x86_64-linux-gnu)
  # Compile without support of ffmpeg
  set(ENGINE_FFMPEG_ENABLED OFF)

  add_library(FFmpeg INTERFACE)
  message(
    "TINYRENDERER >>> COULDN'T FIND FFMPEG on your system. Building without this feature"
  )
else()
  # Compile with support of ffmpeg for decoding and encoding
  set(ENGINE_FFMPEG_ENABLED ON)

  add_library(FFmpeg INTERFACE)
  target_include_directories(
    FFmpeg
    INTERFACE ${AVCODEC_INCLUDE_DIRS}
              ${AVFORMAT_INCLUDE_DIRS}
              ${AVFILTER_INCLUDE_DIRS}
              ${AVDEVICE_INCLUDE_DIRS}
              ${AVUTIL_INCLUDE_DIRS}
              ${SWRESAMPLE_INCLUDE_DIRS}
              ${SWSCALE_INCLUDE_DIRS})
  target_link_libraries(FFmpeg INTERFACE ${AVCODEC_LIB} ${AVFORMAT_LIB}
                                         ${AVUTIL_LIB} swresample swscale)
  message(
    "TINYRENDERER >>> FOUND FFMPEG on your system. Building with this feature ENABLED"
  )
endif()
message(
  "***************************************************************************************"
)
# ##############################################################################

set(TINYRENDERER_SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/CCommon.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/CMath.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/app/CApplication.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/assets/CTextureManager.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/CFixedCamera.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/CFpsCamera.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/CICamera.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/COrbitCamera.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CFrameBuffer.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CTexture.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CTextureCube.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CIndexBuffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CVertexBuffer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CVertexBufferLayout.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CVertexArray.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/window/CIWindow.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/window/CWindowGLFW.cpp" # use GLFW to
                                                             # create a window
    "${CMAKE_CURRENT_SOURCE_DIR}/src/window/CWindowEGL.cpp" # use EGL to create
                                                            # a "window"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CFog.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CIRenderable.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CMesh.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CMeshBuilder.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CModel.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CScene.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CShadowMap.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CSkybox.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/CImGuiLayer.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/CImGuiManager.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/CImGuiUtilsLayer.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/input/CInputEvent.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/input/CInputManager.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CDirectionalLight.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CILight.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CPointLight.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CSpotLight.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/materials/CMaterial.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CMainRenderer.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CMeshRenderer.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CRenderersCommon.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CSkyboxRenderer.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/CShader.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/CProgram.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/CShaderManager.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CDebugDrawer.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CObjectPicker.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CVideoCommon.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CVideoDecoder.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CVideoEncoder.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/gl.c")

# Configuration previous to creating the target
# ##############################################################################
if(ENGINE_GLFW_ENABLED)
  include_directories(${GLFW_INCLUDE_DIRS})
endif()

if(ENGINE_EGL_ENABLED)
  set(TINYRENDERER_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/egl.c"
                        "${TINYRENDERER_SRCS}")
endif()

# ##############################################################################

add_library(TinyRendererCpp ${TINYRENDERER_SRCS})
target_link_libraries(
  tinyrenderer_cpp_lib
  OpenGL::GL
  tinymath_cpp_lib
  tinyutils_cpp_lib
  assimp
  spdlog
  imgui
  FFmpeg)

# Configuration after the target has been created
# ##############################################################################
if(ENGINE_EGL_ENABLED)
  target_link_libraries(tinyrenderer_cpp_lib OpenGL::EGL)
  add_definitions(-DENGINE_GLFW_ENABLED=1)
endif()
if(ENGINE_GLFW_ENABLED)
  target_link_libraries(tinyrenderer_cpp_lib ${GLFW_LIBRARIES})
  add_definitions(-DENGINE_EGL_ENABLED=1)
endif()
if(ENGINE_FFMPEG_ENABLED)
  add_definitions(-DENGINE_FFMPEG_ENABLED=1)
endif()
if(ENGINE_LOGS_ENABLED)
  add_definitions(-DENGINE_LOGS_ENABLED=1)
endif()
if(ENGINE_PROFILING_ENABLED)
  add_definitions(-DENGINE_PROFILING_ENABLED=1)
endif()
if(ENGINE_TRACK_ALLOCS_ENABLED)
  add_definitions(-DENGINE_TRACK_ALLOCS_ENABLED=1)
endif()
# ##############################################################################

# For docs-generation to use
set_property(GLOBAL PROPERTY TINYRENDERER_INCS_DIR
                             "${CMAKE_CURRENT_SOURCE_DIR}/include")

# add_subdirectory(python/tinyrenderer/bindings) add_subdirectory(examples)
# add_subdirectory(docs)
