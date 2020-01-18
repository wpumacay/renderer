#pragma once

#include <memory>

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace std
{
    // make_unique implementation from Herb Sutter's blog
    // url: https://herbsutter.com/gotw/_102/
    template<typename T, typename ...Args>
    std::unique_ptr<T> make_unique( Args&& ...args )
    {
        return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
    }
}

#endif /* DOXYGEN_SHOULD_SKIP_THIS */