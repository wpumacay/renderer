cmake_minimum_required( VERSION 3.0.0 )

#### aka Cat1 project (=ↀωↀ=)
project( TinyRenderer )
message( "|---------------------------------------------------------|" )
message( "|                  TINY RENDERING ENGINE                  |" )
message( "|---------------------------------------------------------|" )
message( "
                      |\\      _,,,---,,_
                ZZZzz /,`.-'`'    -.  ;-;;,_
                     |,4-  ) )-,_. ,\\ (  `'-'
                    '---''(_/--'  `-'\\_)" )
# ASCII art from: https://www.asciiart.eu/animals/cats

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

option( TINYRENDERER_BUILD_HEADLESS_EGL "Build in headless mode using EGL" OFF )
option( TINYRENDERER_BUILD_DOCS "Build documentation (requires Doxygen)" ON )
option( TINYRENDERER_BUILD_EXAMPLES "Build c/c++ examples" ON )
option( TINYRENDERER_BUILD_PYTHON_BINDINGS "Build python bindings" ON )
option( TINYRENDERER_BUILD_WITH_LOGS "Build using logging functionality" ON )
option( TINYRENDERER_BUILD_WITH_TRACK_ALLOCS "Build with obj-allocation tracking" ON )

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
     "${CMAKE_SOURCE_DIR}/ext/tinymath/include"
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

if ( TINYRENDERER_BUILD_HEADLESS_EGL )
    set( ENGINE_HEADLESS_EGL "YES" )
    add_definitions( -DENGINE_HEADLESS_EGL )
else()
    set( ENGINE_HEADLESS_EGL "NO" )
endif()

if ( TINYRENDERER_BUILD_WITH_LOGS OR ( CMAKE_BUILD_TYPE STREQUAL Debug ) )
    set( ENGINE_USE_LOGS "YES" )
    add_definitions( -DENGINE_USE_LOGS )
else()
    set( ENGINE_USE_LOGS "NO" )
endif()

if ( TINYRENDERER_BUILD_WITH_TRACK_ALLOCS OR ( CMAKE_BUILD_TYPE STREQUAL Debug ) )
    set( ENGINE_TRACK_ALLOCS "YES" )
    add_definitions( -DENGINE_TRACK_ALLOCS )
else()
    set( ENGINE_TRACK_ALLOCS "NO" )
endif()

if ( NOT TINYRENDERER_BUILD_HEADLESS_EGL )
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

    set( SRCS
         "${CMAKE_CURRENT_SOURCE_DIR}/src/CCommon.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/CMath.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/app/CApplication.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/assets/CTextureManager.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/CFixedCamera.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/CFpsCamera.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/CICamera.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/COrbitCamera.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CFrameBuffer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CIndexBuffer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CTexture.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CTextureCube.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CVertexArray.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CVertexBuffer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CVertexBufferLayout.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/CIWindow.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/CWindowGLFW.cpp" # use GLFW to create a window
         "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CFog.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CIRenderable.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CMesh.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CMeshBuilder.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CModel.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CScene.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CShadowMap.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CSkybox.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/CImGuiLayer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/CImGuiManager.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/gui/CImGuiUtilsLayer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/input/CInputEvent.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/input/CInputManager.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CDirectionalLight.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CILight.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CPointLight.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CSpotLight.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/materials/CMaterial.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CMainRenderer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CMeshRenderer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CRenderersCommon.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CSkyboxRenderer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/CShader.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/CShaderManager.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CDebugDrawer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CLogger.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CNoiseGenerator.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CObjectPicker.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CProfiling.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CTime.cpp" )

    add_library( tinyrenderer_cpp_lib SHARED ${SRCS} )
    target_link_libraries( tinyrenderer_cpp_lib
                           ${GLFW_LIBRARIES}
                           ${OPENGL_LIBRARIES}
                           ${GLEW_LIBRARIES}
                           tinymath_cpp_lib
                           assimp
                           spdlog
                           imgui )
else()
    # Use EGL and EGL-specific OpengGL requirements
    find_package( OpenGL REQUIRED OpenGL EGL )
    message( "OPENGL_LIBRARIES      : ${OPENGL_LIBRARIES}" )

    set( SRCS
         "${CMAKE_CURRENT_SOURCE_DIR}/src/CCommon.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/CMath.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/app/CApplication.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/assets/CTextureManager.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/CFixedCamera.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/CFpsCamera.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/CICamera.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/camera/COrbitCamera.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CFrameBuffer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CIndexBuffer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CTexture.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CTextureCube.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CVertexArray.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CVertexBuffer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/core/CVertexBufferLayout.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/CIWindow.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/gl/CWindowEGL.cpp" # use EGL to create a "window"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CFog.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CIRenderable.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CMesh.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CMeshBuilder.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CModel.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CScene.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CShadowMap.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/graphics/CSkybox.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/input/CInputEvent.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/input/CInputManager.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CDirectionalLight.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CILight.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CPointLight.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/lights/CSpotLight.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/materials/CMaterial.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CMainRenderer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CMeshRenderer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CRenderersCommon.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/renderers/CSkyboxRenderer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/CShader.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/CShaderManager.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CDebugDrawer.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CLogger.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CNoiseGenerator.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CObjectPicker.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CProfiling.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/src/utils/CTime.cpp"
         "${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/egl.c"
         "${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/gl.c" )

    add_library( tinyrenderer_cpp_lib SHARED ${SRCS} )
    target_link_libraries( tinyrenderer_cpp_lib
                           OpenGL::OpenGL
                           OpenGL::EGL
                           tinymath_cpp_lib
                           assimp
                           spdlog
                           dl )
endif()

set_property( TARGET tinyrenderer_cpp_lib PROPERTY TINYRENDERER_INCS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include" )

if ( TINYRENDERER_BUILD_PYTHON_BINDINGS )
    add_subdirectory( python/tinyrenderer/bindings )
endif()

if ( TINYRENDERER_IS_MASTER_PROJECT AND TINYRENDERER_BUILD_DOCS )
    add_subdirectory( docs )
endif()

if ( TINYRENDERER_IS_MASTER_PROJECT AND TINYRENDERER_BUILD_EXAMPLES )
    add_subdirectory( examples )
endif()

message( "***************************************************************************************" )
message( "BUILD-OPTIONS SUMMARY:" )
message( "CMAKE_BUILD_TYPE                      : ${CMAKE_BUILD_TYPE}" )
message( "CMAKE_LIBRARY_OUTPUT_DIRECTORY        : ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}" )
message( "CMAKE_BUILD_RPATH                     : ${CMAKE_BUILD_RPATH}" )
message( "TINYRENDERER_RESOURCES_PATH           : ${TINYRENDERER_RESOURCES_PATH}" )
message( "TINYRENDERER_BUILD_HEADLESS_EGL       : ${TINYRENDERER_BUILD_HEADLESS_EGL}" )
message( "TINYRENDERER_BUILD_DOCS               : ${TINYRENDERER_BUILD_DOCS}" )
message( "TINYRENDERER_BUILD_EXAMPLES           : ${TINYRENDERER_BUILD_EXAMPLES}" )
message( "TNIYRENDERER_BUILD_PYTHON_BINDINGS    : ${TINYRENDERER_BUILD_PYTHON_BINDINGS}" )
message( "TINYRENDERER_BUILD_WITH_LOGS          : ${TINYRENDERER_BUILD_WITH_LOGS}" )
message( "ENGINE_USE_LOGS                       : ${ENGINE_USE_LOGS}" )
message( "ENGINE_TRACK_ALLOCS                   : ${ENGINE_TRACK_ALLOCS}" )
message( "***************************************************************************************" )