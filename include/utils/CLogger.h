#pragma once

#include <cstdlib>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace engine
{
    using string_view_t = fmt::basic_string_view<char>;

    class CLogger
    {
    public :
        static void Init();
        static void Release();
        static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

        template< typename... Args >
        static void CoreTrace( string_view_t fmt, const Args &... args )
        {
            if ( !CLogger::s_Initialized )
                return;

            CLogger::s_CoreLogger->trace( fmt, args... );
        }

        template< typename... Args >
        static void CoreInfo( string_view_t fmt, const Args &... args )
        {
            if ( !CLogger::s_Initialized )
                return;

            CLogger::s_CoreLogger->info( fmt, args... );
        }

        template< typename... Args >
        static void CoreWarn( string_view_t fmt, const Args &... args )
        {
            if ( !CLogger::s_Initialized )
                return;

            CLogger::s_CoreLogger->warn( fmt, args... );
        }

        template< typename... Args >
        static void CoreError( string_view_t fmt, const Args &... args )
        {
            if ( !CLogger::s_Initialized )
                return;

            CLogger::s_CoreLogger->error( fmt, args... );
        }

        template< typename... Args >
        static void CoreCritical( string_view_t fmt, const Args &... args )
        {
            if ( !CLogger::s_Initialized )
                return;

            CLogger::s_CoreLogger->critical( fmt, args... );
        }

        template< typename... Args >
        static void ClientTrace( string_view_t fmt, const Args &... args )
        {
            if ( !CLogger::s_Initialized )
                return;

            CLogger::s_ClientLogger->trace( fmt, args... );
        }

        template< typename... Args >
        static void ClientInfo( string_view_t fmt, const Args &... args )
        {
            if ( !CLogger::s_Initialized )
                return;

            CLogger::s_ClientLogger->info( fmt, args... );
        }

        template< typename... Args >
        static void ClientWarn( string_view_t fmt, const Args &... args )
        {
            if ( !CLogger::s_Initialized )
                return;

            CLogger::s_ClientLogger->warn( fmt, args... );
        }

        template< typename... Args >
        static void ClientError( string_view_t fmt, const Args &... args )
        {
            if ( !CLogger::s_Initialized )
                return;

            CLogger::s_ClientLogger->error( fmt, args... );
        }

        template< typename... Args >
        static void ClientCritical( string_view_t fmt, const Args &... args )
        {
            if ( !CLogger::s_Initialized )
                return;

            CLogger::s_ClientLogger->critical( fmt, args... );
        }

    private :
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;

        static bool s_Initialized;
    };

    inline void empty_func() {}
}

#if defined( ENGINE_USE_LOGS )
    #define ENGINE_CORE_TRACE(...)      ::engine::CLogger::CoreTrace(__VA_ARGS__)
    #define ENGINE_CORE_INFO(...)       ::engine::CLogger::CoreInfo(__VA_ARGS__)
    #define ENGINE_CORE_WARN(...)       ::engine::CLogger::CoreWarn(__VA_ARGS__)
    #define ENGINE_CORE_ERROR(...)      ::engine::CLogger::CoreError(__VA_ARGS__)
    #define ENGINE_CORE_CRITICAL(...)   ::engine::CLogger::CoreCritical(__VA_ARGS__)
    #define ENGINE_CORE_ASSERT(x, ...) \
        {                                                                               \
            if( !x )                                                                    \
            {                                                                           \
                ENGINE_CORE_CRITICAL( "Failed due to assertion: {0}", __VA_ARGS__ );    \
                exit( EXIT_FAILURE );                                                   \
            }                                                                           \
        }

    #define ENGINE_TRACE(...)       ::engine::CLogger::ClientTrace(__VA_ARGS__)
    #define ENGINE_INFO(...)        ::engine::CLogger::ClientInfo(__VA_ARGS__)
    #define ENGINE_WARN(...)        ::engine::CLogger::ClientWarn(__VA_ARGS__)
    #define ENGINE_ERROR(...)       ::engine::CLogger::ClientError(__VA_ARGS__)
    #define ENGINE_CRITICAL(...)    ::engine::CLogger::ClientCritical(__VA_ARGS__)
    #define ENGINE_ASSERT(x, ...) \
        {                                                                       \
            if( !x )                                                            \
            {                                                                   \
                ENGINE_CRITICAL( "Failed due to assertion: {0}", __VA_ARGS__ ); \
                exit( EXIT_FAILURE );                                           \
            }                                                                   \
        }
#else
    #define ENGINE_CORE_TRACE(...) ((void)0)
    #define ENGINE_CORE_INFO(...) ((void)0)
    #define ENGINE_CORE_WARN(...) ((void)0)
    #define ENGINE_CORE_ERROR(...) ((void)0)
    #define ENGINE_CORE_CRITICAL(...) ((void)0)
    #define ENGINE_CORE_ASSERT(x, ...) \
        {                               \
            if( !x )                    \
            {                           \
                exit( EXIT_FAILURE );   \
            }                           \
        }

    #define ENGINE_TRACE(...) ((void)0)
    #define ENGINE_INFO(...) ((void)0)
    #define ENGINE_WARN(...) ((void)0)
    #define ENGINE_ERROR(...) ((void)0)
    #define ENGINE_CRITICAL(...) ((void)0)
    #define ENGINE_ASSERT(x, ...) \
        {                               \
            if( !x )                    \
            {                           \
                exit( EXIT_FAILURE );   \
            }                           \
        }
#endif