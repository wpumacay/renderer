#pragma once

#include <GL/glew.h>
#include <cassert>
#include <cmath>
#include <csignal>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <memory>
#include <algorithm>
#include <functional>
#include <vector>
#include <unordered_map>
#include <map>

#define ENGINE_PI 3.141592653589793
#define ENGINE_EPS 0.000001

#ifndef ENGINE_RESOURCES_PATH
    #define ENGINE_RESOURCES_PATH "../res/"
#endif

namespace engine
{

    std::vector< std::string > split( const std::string &txt, char separator = '.' );

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

    enum class ePixelFormat
    {
        NONE = 0, 
        RGB, 
        RGBA
    };

    std::string toString( const ePixelFormat& format );
    uint32 toOpenGLEnum( const ePixelFormat& format );

}