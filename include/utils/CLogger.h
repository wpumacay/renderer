#pragma once

#include <LCommon.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace engine
{
    class CLogger
    {
    public :
        static void Init();
        static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private :
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };
}

#define ENGINE_CORE_TRACE(...)      ::engine::CLogger::GetCoreLogger()->trace(__VA_ARGS__)
#define ENGINE_CORE_INFO(...)       ::engine::CLogger::GetCoreLogger()->info(__VA_ARGS__)
#define ENGINE_CORE_WARN(...)       ::engine::CLogger::GetCoreLogger()->warn(__VA_ARGS__)
#define ENGINE_CORE_ERROR(...)      ::engine::CLogger::GetCoreLogger()->error(__VA_ARGS__)
#define ENGINE_CORE_CRITICAL(...)   ::engine::CLogger::GetCoreLogger()->critical(__VA_ARGS__)

#define ENGINE_CORE_ASSERT(x, ...) \
    {                                                                           \
        if( !x )                                                                \
        {                                                                       \
            ENGINE_CORE_CRITICAL( "Failed due to assertion {0}", __VA_ARGS__ ); \
            assert( false );                                                    \
        }                                                                       \
    }

#define ENGINE_TRACE(...)       ::engine::CLogger::GetClientLogger()->trace(__VA_ARGS__)
#define ENGINE_INFO(...)        ::engine::CLogger::GetClientLogger()->info(__VA_ARGS__)
#define ENGINE_WARN(...)        ::engine::CLogger::GetClientLogger()->warn(__VA_ARGS__)
#define ENGINE_ERROR(...)       ::engine::CLogger::GetClientLogger()->error(__VA_ARGS__)
#define ENGINE_CRITICAL(...)    ::engine::CLogger::GetClientLogger()->critical(__VA_ARGS__)

#define ENGINE_ASSERT(x, ...) \
    {                                                                       \
        if( !x )                                                            \
        {                                                                   \
            ENGINE_CRITICAL( "Failed due to assertion {0}", __VA_ARGS__ );  \
            assert( false );                                                \
        }                                                                   \
    }
