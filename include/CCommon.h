#pragma once

#include <GL/glew.h>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <random>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <memory>
#include <csignal>
#include <functional>

#define ENGINE_PI 3.141592653589793
#define ENGINE_EPS 0.000001

#ifndef ENGINE_RESOURCES_PATH
    #define ENGINE_RESOURCES_PATH "../res/"
#endif

namespace engine
{

    enum class eAxis
    {
        X = 0, Y, Z
    };

    std::string toString( const eAxis& axis );

    typedef unsigned char u8;
    typedef unsigned int uint32;
    typedef unsigned long uint64;
    typedef float float32;
    typedef double float64;

}