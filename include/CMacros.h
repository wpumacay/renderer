#pragma once

#ifndef DOXYGEN_SHOULD_SKIP_THIS

#if defined( __GNUC__ )
    #define __FUNCTION_NAME__ __PRETTY_FUNCTION__
#elif defined(__clang__)
    #define __FUNCTION_NAME__ __FUNCTION__
#elif defined(_MSC_VER)
    #define __FUNCTION_NAME__ __FUNCSIG__
#endif

#endif /* DOXYGEN_SHOULD_SKIP_THIS */