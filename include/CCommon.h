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

#define ENGINE_PI 3.141592653589793f
#define ENGINE_EPS 0.000001f

#ifndef ENGINE_RESOURCES_PATH
    #define ENGINE_RESOURCES_PATH "../res/"
#endif

#ifndef ENGINE_EXAMPLES_PATH
    #define ENGINE_EXAMPLES_PATH "../../../examples/"
#endif

namespace engine
{
    std::vector< std::string > split( const std::string &txt, char separator = '.' );

    std::string getFilenameFromFilePath( const std::string& filepath );
    std::string getFoldernameFromFilePath( const std::string& filepath );
    std::string getFolderpathFromFilePath( const std::string& filepath );
    std::string getFilenameNoExtensionFromFilePath( const std::string& filepath );

    std::string pointerToHexAddress( const void* ptr );

    typedef unsigned char uint8;
    typedef unsigned int uint32;
    typedef unsigned long uint64;
    typedef int int32;
    typedef long int64;
    typedef float float32;
    typedef double float64;

    enum class eAxis
    {
        X = 0, Y, Z
    };

    std::string toString( const eAxis& axis );

    enum class ePixelDataType
    {
        NONE = 0,
        UINT_8,
        UINT_32,
        UINT_24_8,
        FLOAT_32
    };

    std::string toString( const ePixelDataType& dtype );
    uint32 toOpenGLEnum( const ePixelDataType& dtype );
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