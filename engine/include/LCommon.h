
#pragma once

#include <iostream>
#include <GL/glew.h>

#include "LConfig.h"

#ifdef ENGINE_ENABLE_LOGS
    #define ENGINE_LOG( X ) std::cout << X << std::endl
#else
    #define ENGINE_LOG( X ) 
#endif

#ifdef ENGINE_ENABLE_ERRORS
    #define ENGINE_ERROR( X ) std::cerr << X << std::endl
#else
    #define ENGINE_ERROR( X )
#endif


#define ENGINE_OVERRIDE_APP( APP )


namespace engine
{

    namespace common
    {




















    }






}