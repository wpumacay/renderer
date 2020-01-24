#pragma once

#include <GL/glew.h>
#include <array>
#include <cassert>
#include <cmath>
#include <csignal>
#include <cstdlib>
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

#include <CExtensions.h>

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
