cmake_minimum_required( VERSION 3.0.0 )

#### aka Cat1 project (=ↀωↀ=)
project( TinyRenderer )

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wno-format-security -Wno-endif-labels -fPIC" )

# In case nobody set the project type, set it (parent project might have set it for all)
if ( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Debug )
endif()

# Compile everything as shared libraries
set( BUILD_SHARED_LIBS ON CACHE BOOL "Build libraries as shared" )

# Outputs of the build should be sent accordingly
if ( NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY )
    message( "TINYRENDERER >>> Sending generated libs to OWN build directory" )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/libs" )
else()
    message( "TINYRENDERER >>> Sending generated libs to EXTERNAL build directory" )
endif()
message( "TINYRENDERER >>> LIBRARY_OUT_DIR: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" )

set( TINYRENDERER_IS_MASTER_PROJECT OFF )
if( CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( "TINYRENDERER >>> Configuring as MASTER project" )
    set( TINYRENDERER_IS_MASTER_PROJECT ON )
else()
    message( "TINYRENDERER >>> Configuring as CHILD project" )
endif()

option( TINYRENDERER_BUILD_WINDOWED_GLFW "Build in normal windowed mode using GLFW" ON )
option( TINYRENDERER_BUILD_HEADLESS_EGL "Build in headless mode using EGL" OFF )
option( TINYRENDERER_BUILD_DOCS "Build documentation (requires Doxygen)" OFF )
option( TINYRENDERER_BUILD_EXAMPLES "Build c/c++ examples" ON )
option( TINYRENDERER_BUILD_PYTHON_BINDINGS "Build python bindings" ON )
option( TINYRENDERER_BUILD_WITH_LOGS "Build using logging functionality" ON )
option( TINYRENDERER_BUILD_WITH_PROFILING "Build using profiling tools enabled" OFF )
option( TINYRENDERER_BUILD_WITH_TRACK_ALLOCS "Build with obj-allocation tracking" OFF )
option( TINYRENDERER_BUILD_WITH_FFMPEG_UTILS "Build ffmpeg utils to read and write to video files" OFF )

# Add cmake find-modules helpers (for sphinx)
if( TINYRENDERER_IS_MASTER_PROJECT )
    set( CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH} )
endif()

# Add external dependencies only if master project. If child project, root adds the dependencies
if ( TINYRENDERER_IS_MASTER_PROJECT )
    add_subdirectory( ext )
endif()

# Assimp is required in both normal and headless modes
find_package( ASSIMP REQUIRED )

if ( APPLE )
    include_directories( /usr/local/include )
    link_directories( /usr/local/lib )
endif()

message( "ASSIMP_INCLUDE_DIRS   : ${ASSIMP_INCLUDE_DIRS}" )

set( TINYRENDERER_INCLUDES_DIRS
     "${CMAKE_CURRENT_SOURCE_DIR}/include"
     "${CMAKE_CURRENT_SOURCE_DIR}/ext/stb"
     "${CMAKE_SOURCE_DIR}/ext/tiny_math/include"
     "${CMAKE_SOURCE_DIR}/ext/tiny_utils/include"
     "${CMAKE_SOURCE_DIR}/ext/imgui"
     "${CMAKE_SOURCE_DIR}/ext/imgui/examples"
     "${CMAKE_SOURCE_DIR}/ext/spdlog/include" )
include_directories( ${TINYRENDERER_INCLUDES_DIRS} )
if ( TINYRENDERER_BUILD_HEADLESS_EGL )
    include_directories( "${CMAKE_CURRENT_SOURCE_DIR}/ext/glad" )
endif()

if ( NOT TINYRENDERER_RESOURCES_PATH )
    set( TINYRENDERER_RESOURCES_PATH "${CMAKE_CURRENT_SOURCE_DIR}/res/" )
endif()

add_definitions( -DENGINE_RESOURCES_PATH="${TINYRENDERER_RESOURCES_PATH}")
add_definitions( -DENGINE_EXAMPLES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/examples/cpp/" )

if ( (NOT TINYRENDERER_BUILD_WINDOWED_GLFW) AND (NOT TINYRENDERER_BUILD_HEADLESS_EGL) )
    message( "TINYRENDERER >>> at least one 'window' mode should be enabled : GLFW, EGL or both" )
    return()
endif()

if ( TINYRENDERER_BUILD_WITH_LOGS )
    set( ENGINE_USE_LOGS "YES" )
    add_definitions( -DENGINE_USE_LOGS )
else()
    set( ENGINE_USE_LOGS "NO" )
endif()

if ( TINYRENDERER_BUILD_WITH_PROFILING )
    set( ENGINE_USE_PROFILING "YES" )
    add_definitions( -DENGINE_USE_PROFILING )
else()
    set( ENGINE_USE_PROFILING "NO" )
endif()

if ( TINYRENDERER_BUILD_WITH_TRACK_ALLOCS )
    set( ENGINE_TRACK_ALLOCS "YES" )
    add_definitions( -DENGINE_TRACK_ALLOCS )
else()
    set( ENGINE_TRACK_ALLOCS "NO" )
endif()

if ( TINYRENDERER_BUILD_WITH_FFMPEG_UTILS )
    # Search libav libraries
    find_path( AVCODEC_INCLUDE_DIRS     libavcodec/avcodec.h )
    find_path( AVFORMAT_INCLUDE_DIRS    libavformat/avformat.h )
    find_path( AVUTIL_INCLUDE_DIRS      libavutil/avutil.h )
    find_path( AVDEVICE_INCLUDE_DIRS    libavdevice/avdevice.h )
    find_path( SWRESAMPLE_INCLUDE_DIRS  libswresample/swresample.h )
    find_path( SWSCALE_INCLUDE_DIRS     libswscale/swscale.h )

    find_library( AVCODEC_LIB       avcodec )
    find_library( AVFORMAT_LIB      avformat )
    find_library( AVUTIL_LIB        avutil )
    find_library( AVDEVICE_LIB      avdevice )
    find_library( SWRESAMPLE_LIB    swresample )
    find_library( SWSCALE_LIB       swscale )

    message( "AVCODEC_INCLUDE_DIRS   : ${AVCODEC_INCLUDE_DIRS}" )
    message( "AVFORMAT_INCLUDE_DIRS  : ${AVFORMAT_INCLUDE_DIRS}" )
    message( "AVUTIL_INCLUDE_DIRS    : ${AVUTIL_INCLUDE_DIRS}" )
    message( "AVDEVICE_INCLUDE_DIRS  : ${AVDEVICE_INCLUDE_DIRS}" )
    message( "SWRESAMPLE_INCLUDE_DIRS: ${SWRESAMPLE_INCLUDE_DIRS}" )
    message( "SWSCALE_INCLUDE_DIRS   : ${SWSCALE_INCLUDE_DIRS}" )

    message( "AVCODEC_LIB   : ${AVCODEC_LIB}" )
    message( "AVFORMAT_LIB  : ${AVFORMAT_LIB}" )
    message( "AVUTIL_LIB    : ${AVUTIL_LIB}" )
    message( "AVDEVICE_LIB  : ${AVDEVICE_LIB}" )
    message( "SWRESAMPLE_LIB: ${SWRESAMPLE_LIB}" )
    message( "SWSCALE_LIB   : ${SWSCALE_LIB}" )

    if ( AVCODEC_LIB STREQUAL "AVCODEC_LIB-NOTFOUND" OR AVFORMAT_LIB STREQUAL "AVFORMAT_LIB-NOTFOUND" OR
         AVUTIL_LIB STREQUAL "AVUTIL_LIB-NOTFOUND" OR AVDEVICE_LIB STREQUAL "AVDEVICE_LIB-NOTFOUND" OR
         SWRESAMPLE_LIB STREQUAL "SWRESAMPLE_LIB-NOTFOUND" OR SWSCALE_LIB STREQUAL "SWSCALE_LIB-NOTFOUND" )
        set( FFMPEG_FOUND OFF ) # @todo: check alternative using the old method (pkg-config + hard-coded /usr/lib/x86_64-linux-gnu)
        # Compile without support of ffmpeg
        set( ENGINE_USE_FFMPEG "NO" )

        add_library( FFmpeg INTERFACE )
        message( "TINYRENDERER >>> COULDN'T FIND FFMPEG on your system. Building without this feature" )
    else()
        # Compile with support of ffmpeg for decoding and encoding
        set( ENGINE_USE_FFMPEG "YES" )
        add_definitions( -DENGINE_USE_FFMPEG )

        add_library( FFmpeg INTERFACE )
        target_include_directories( FFmpeg INTERFACE
            ${AVCODEC_INCLUDE_DIRS}
            ${AVFORMAT_INCLUDE_DIRS}
            ${AVFILTER_INCLUDE_DIRS}
            ${AVDEVICE_INCLUDE_DIRS}
            ${AVUTIL_INCLUDE_DIRS}
            ${SWRESAMPLE_INCLUDE_DIRS}
            ${SWSCALE_INCLUDE_DIRS} )
        target_link_libraries( FFmpeg INTERFACE
            ${AVCODEC_LIB} ${AVFORMAT_LIB} ${AVUTIL_LIB} swresample swscale )
    endif()
else()
    set( ENGINE_USE_FFMPEG "NO" )
    add_library( FFmpeg INTERFACE )
endif()

# Default states (will properly set them below)
set( ENGINE_WINDOWED_GLFW "NO" )
set( ENGINE_HEADLESS_EGL "NO" )

if ( TINYRENDERER_BUILD_WINDOWED_GLFW )
    message( "TINYRENDERER >>> Configuring windowed mode using GLFW" )
    # Use GLFW and default OpengGL requirements
    find_package( PkgConfig REQUIRED )
    find_package( OpenGL REQUIRED )
    find_package( GLEW REQUIRED )
    pkg_search_module( GLFW REQUIRED glfw3 )
    message( "GLFW_INCLUDE_DIRS     : ${GLFW_INCLUDE_DIRS}" )
    message( "GLFW_LIBRARIES        : ${GLFW_LIBRARIES}" )
    message( "GLFW_LIBRARY_DIRS     : ${GLFW_LIBRARY_DIRS}" )
    message( "GLEW_INCLUDE_DIRS     : ${GLEW_INCLUDE_DIRS}" )
    message( "GLEW_LIBRARIES        : ${GLEW_LIBRARIES}" )
    message( "OPENGL_LIBRARIES      : ${OPENGL_LIBRARIES}" )
    include_directories( ${GLFW_INCLUDE_DIRS} )
    include_directories( ${GLEW_INCLUDE_DIRS} )

    set( SRCS_WINDOWED_GLFW
         "${CMAKE_CURRENT_SOURCE_DIR}/src/CCommon.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/CMath.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/app/CApplication.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/assets/CTextureManager.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/CFixedCamera.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/CFpsCamera.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/CICamera.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/COrbitCamera.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CFrameBuffer.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CTexture.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CTextureCube.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CIndexBuffer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CVertexBuffer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CVertexBufferLayout.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CVertexArray.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/window/CIWindow.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/window/CWindowGLFW.cpp" # use GLFW to create a window
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CFog.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CIRenderable.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CMesh.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CMeshBuilder.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CModel.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CScene.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CShadowMap.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CSkybox.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/CImGuiLayer.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/CImGuiManager.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/CImGuiUtilsLayer.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/input/CInputEvent.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/input/CInputManager.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CDirectionalLight.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CILight.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CPointLight.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CSpotLight.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/materials/CMaterial.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CMainRenderer.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CMeshRenderer.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CRenderersCommon.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CSkyboxRenderer.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/CShader.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/CShaderManager.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CDebugDrawer.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CObjectPicker.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CVideoCommon.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CVideoDecoder.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CVideoEncoder.cpp"
          )
    set( ENGINE_WINDOWED_GLFW "YES" )

    add_library( tinyrenderer_cpp_lib_glfw SHARED ${SRCS_WINDOWED_GLFW} )
    target_link_libraries( tinyrenderer_cpp_lib_glfw
                           ${GLFW_LIBRARIES}
                           ${OPENGL_LIBRARIES}
                           ${GLEW_LIBRARIES}
                           tinymath_cpp_lib
                           tinyutils_cpp_lib
                           assimp
                           spdlog
                           imgui
                           FFmpeg )
    target_compile_definitions( tinyrenderer_cpp_lib_glfw PRIVATE ENGINE_WINDOWED_GLFW=1 )
endif()

if ( TINYRENDERER_BUILD_HEADLESS_EGL )
    message( "TINYRENDERER >>> Configuring headless mode using EGL" )
    # Use EGL and EGL-specific OpengGL requirements
    find_package( OpenGL REQUIRED OpenGL EGL )
    message( "OPENGL_LIBRARIES      : ${OPENGL_LIBRARIES}" )

    set( SRCS_HEADLESS_EGL
         "${CMAKE_CURRENT_SOURCE_DIR}/src/CCommon.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/CMath.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/app/CApplication.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/assets/CTextureManager.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/CFixedCamera.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/CFpsCamera.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/CICamera.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/COrbitCamera.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CFrameBuffer.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CTexture.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CTextureCube.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CIndexBuffer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CVertexBuffer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CVertexBufferLayout.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CVertexArray.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/window/CIWindow.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/window/CWindowEGL.cpp" # use EGL to create a "window"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CFog.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CIRenderable.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CMesh.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CMeshBuilder.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CModel.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CScene.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CShadowMap.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CSkybox.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/input/CInputEvent.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/input/CInputManager.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CDirectionalLight.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CILight.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CPointLight.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CSpotLight.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/materials/CMaterial.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CMainRenderer.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CMeshRenderer.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CRenderersCommon.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CSkyboxRenderer.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/CShader.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/CShaderManager.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CDebugDrawer.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CObjectPicker.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CVideoCommon.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CVideoDecoder.cpp"
         #### "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CVideoEncoder.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/egl.c"
         "${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/gl.c"
          )
    set( ENGINE_HEADLESS_EGL "YES" )

    add_library( tinyrenderer_cpp_lib_egl SHARED ${SRCS_HEADLESS_EGL} )
    target_link_libraries( tinyrenderer_cpp_lib_egl
                           OpenGL::OpenGL
                           OpenGL::EGL
                           tinymath_cpp_lib
                           tinyutils_cpp_lib
                           assimp
                           spdlog
                           dl
                           FFmpeg )
    target_compile_definitions( tinyrenderer_cpp_lib_egl PRIVATE ENGINE_HEADLESS_EGL=1 )
endif()

# For docs-generation to use
set_property( GLOBAL PROPERTY TINYRENDERER_INCS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" )

#### if ( TINYRENDERER_IS_MASTER_PROJECT AND TINYRENDERER_BUILD_PYTHON_BINDINGS )
####     add_subdirectory( python/tinyrenderer/bindings )
#### endif()

if ( TINYRENDERER_IS_MASTER_PROJECT AND TINYRENDERER_BUILD_DOCS )
    add_subdirectory( docs )
endif()

if ( TINYRENDERER_IS_MASTER_PROJECT AND TINYRENDERER_BUILD_EXAMPLES )
    add_subdirectory( examples )
endif()
message( "|---------------------------------------------------------|" )
message( "|                  TINY RENDERING ENGINE                  |" )
message( "|---------------------------------------------------------|" )
message( "
                      |\\      _,,,---,,_
                ZZZzz /,`.-'`'    -.  ;-;;,_
                     |,4-  ) )-,_. ,\\ (  `'-'
                    '---''(_/--'  `-'\\_)" )
# ASCII art from: https://www.asciiart.eu/animals/cats
message( "***************************************************************************************" )
message( "BUILD-OPTIONS SUMMARY:" )
message( "CMAKE_BUILD_TYPE                      : ${CMAKE_BUILD_TYPE}" )
message( "CMAKE_LIBRARY_OUTPUT_DIRECTORY        : ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" )
message( "CMAKE_BUILD_RPATH                     : ${CMAKE_BUILD_RPATH}" )
message( "TINYRENDERER_RESOURCES_PATH           : ${TINYRENDERER_RESOURCES_PATH}" )
message( "TINYRENDERER_BUILD_WINDOWED_GLFW      : ${TINYRENDERER_BUILD_WINDOWED_GLFW}" )
message( "TINYRENDERER_BUILD_HEADLESS_EGL       : ${TINYRENDERER_BUILD_HEADLESS_EGL}" )
message( "TINYRENDERER_BUILD_DOCS               : ${TINYRENDERER_BUILD_DOCS}" )
message( "TINYRENDERER_BUILD_EXAMPLES           : ${TINYRENDERER_BUILD_EXAMPLES}" )
message( "TNIYRENDERER_BUILD_PYTHON_BINDINGS    : ${TINYRENDERER_BUILD_PYTHON_BINDINGS}" )
message( "TINYRENDERER_BUILD_WITH_LOGS          : ${TINYRENDERER_BUILD_WITH_LOGS}" )
message( "TINYRENDERER_BUILD_WITH_PROFILING     : ${TINYRENDERER_BUILD_WITH_PROFILING}" )
message( "TINYRENDERER_BUILD_WITH_FFMPEG_UTILS  : ${TINYRENDERER_BUILD_WITH_FFMPEG_UTILS}" )
message( "ENGINE_USE_LOGS                       : ${ENGINE_USE_LOGS}" )
message( "ENGINE_USE_PROFILING                  : ${ENGINE_USE_PROFILING}" )
message( "ENGINE_TRACK_ALLOCS                   : ${ENGINE_TRACK_ALLOCS}" )
message( "ENGINE_USE_FFMPEG                     : ${ENGINE_USE_FFMPEG}" )
message( "***************************************************************************************" )