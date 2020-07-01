#pragma once

#ifdef ENGINE_HEADLESS_EGL
    #include <glad/gl.h>
#else
    #include <GL/glew.h>
#endif
#include <array>
#include <cassert>
#include <cmath>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <memory>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>

#include <tinyutils/common.hpp>
#include <tinyutils/logging.hpp>
#include <tinyutils/timing.hpp>
#include <tinyutils/path_handling.hpp>
#include <tinyutils/perlin_noise.hpp>
#include <tinyutils/profiling.hpp>
#include <CMath.h>

#ifndef ENGINE_RESOURCES_PATH
    #define ENGINE_RESOURCES_PATH "../res/"
#endif

#ifndef ENGINE_EXAMPLES_PATH
    #define ENGINE_EXAMPLES_PATH "../../../examples/"
#endif

namespace engine
{
    /// Path to where the engine resources are located (mesh-files, textures, etc.)
    const std::string RESOURCES_PATH = ENGINE_RESOURCES_PATH;
    /// Path to where the engine examples are stored
    const std::string EXAMPLES_PATH = ENGINE_EXAMPLES_PATH;

    //----------------------------------------------------------------------------------------------

    /// Available axis used for world-up vectors and other references in the codebase
    enum class eAxis
    {
        X, ///< X-axis
        Y, ///< Y-axis
        Z  ///< Z-axis
    };

    /// Returns the string representation of the given axis enum
    ///
    /// @param axis Axis enumerator value
    /// @return String representation of the given axis enum
    std::string ToString( const eAxis& axis );
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------

    /// Available data types for pixel values
    enum class ePixelDataType
    {
        UINT_8,     ///< Pixel value with 1-byte of storage
        UINT_32,    ///< Pixel value with 4-bytes of storage
        UINT_24_8,  ///< Pixel value with 3-bytes + 1-bytes of (separated) storage
        FLOAT_32,   ///< Pixel value with 4-bytes of  storage as a 32-bit floating-point numbet
    };

    /// Returns the string representation of a ePixelDataType enum
    ///
    /// @param dtype Pixel data type storage representation
    /// @return String representation of the data-type representation
    std::string ToString( const ePixelDataType& dtype );

    /// Converts the given pixel data-type representation into its related OpenGL enum
    ///
    /// @param dtype Pixel data type storage representation
    /// @return 
    uint32 ToOpenGLEnum( const ePixelDataType& dtype );
    //----------------------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------------------

    /// Available modes in which a VBO can be used
    enum class eBufferUsage
    {
        /// Buffer holds memory that won't be changed by the user after its creation. Generally this
        /// type of buffer is allocated on GPU such that it can read it quickly
        STATIC = 0,
        /// Buffer holds memory that can be changed by the user on the fly. This type of buffer is
        /// allocated on GPU such that it allows write operations from the CPU, so its slower compared
        /// to the STATIC type of buffer
        DYNAMIC
    };

    /// Returns a string representation of the given BufferUsage enum
    ///
    /// @param usage    Usage type of a given buffer
    /// @return String representation of this usage type
    std::string ToString( const eBufferUsage& usage );

    /// Converts the given buffer-usage enum to its related OpenGL enum
    ///
    /// @param usage    Usage type of a given buffer
    /// @return OpenGL enum related to this usage
    uint32 ToOpenGLEnum( const eBufferUsage& usage );
    //----------------------------------------------------------------------------------------------
}

#if defined(ENGINE_USE_LOGS)
    #define ENGINE_CORE_TRACE(...)     LOG_CORE_TRACE(__VA_ARGS__)
    #define ENGINE_CORE_INFO(...)      LOG_CORE_INFO(__VA_ARGS__)
    #define ENGINE_CORE_WARN(...)      LOG_CORE_WARN(__VA_ARGS__)
    #define ENGINE_CORE_ERROR(...)     LOG_CORE_ERROR(__VA_ARGS__)
    #define ENGINE_CORE_CRITICAL(...)  LOG_CORE_CRITICAL(__VA_ARGS__)
    #define ENGINE_CORE_ASSERT(x,...)  LOG_CORE_ASSERT((x), __VA_ARGS__)

    #define ENGINE_TRACE(...)       LOG_TRACE(__VA_ARGS__)
    #define ENGINE_INFO(...)        LOG_INFO(__VA_ARGS__)
    #define ENGINE_WARN(...)        LOG_WARN(__VA_ARGS__)
    #define ENGINE_ERROR(...)       LOG_ERROR(__VA_ARGS__)
    #define ENGINE_CRITICAL(...)    LOG_CRITICAL(__VA_ARGS__)
    #define ENGINE_ASSERT(x,...)    LOG_ASSERT((x), __VA_ARGS__)
#else
    #define ENGINE_CORE_TRACE(...)     ((void)0)
    #define ENGINE_CORE_INFO(...)      ((void)0)
    #define ENGINE_CORE_WARN(...)      ((void)0)
    #define ENGINE_CORE_ERROR(...)     ((void)0)
    #define ENGINE_CORE_CRITICAL(...)   \
    {                                   \
        assert( false );                \
    }
    #define ENGINE_CORE_ASSERT(x,...)   \
    {                                   \
        assert((x));                    \
    }

    #define ENGINE_TRACE(...)       ((void)0)
    #define ENGINE_INFO(...)        ((void)0)
    #define ENGINE_WARN(...)        ((void)0)
    #define ENGINE_ERROR(...)       ((void)0)
    #define ENGINE_CRITICAL(...)    \
    {                               \
        assert( false );            \
    }
    #define ENGINE_ASSERT(x,...)    \
    {                               \
        assert((x));                \
    }
#endif

#if defined(ENGINE_USE_PROFILING)
    #define ENGINE_PROFILE_SCOPE(name)                          PROFILE_SCOPE(name)
    #define ENGINE_PROFILE_SCOPE_IN_SESSION(name, session_name) PROFILE_SCOPE_IN_SESSION(name, session_name)
    #define ENGINE_PROFILE_FUNCTION()                           PROFILE_FUNCTION()
    #define ENGINE_PROFILE_FUNCTION_IN_SESSION(session_name)    PROFILE_FUNCTION_IN_SESSION(session_name)
#else
    #define ENGINE_PROFILE_SCOPE(name)                          ((void)0)
    #define ENGINE_PROFILE_SCOPE_IN_SESSION(name, session_name) ((void)0)
    #define ENGINE_PROFILE_FUNCTION()                           ((void)0)
    #define ENGINE_PROFILE_FUNCTION_IN_SESSION(session_name)    ((void)0)
#endif