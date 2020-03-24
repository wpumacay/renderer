
#include <utils/CLogger.h>

namespace engine
{

    std::shared_ptr<spdlog::logger> CLogger::s_CoreLogger;
    std::shared_ptr<spdlog::logger> CLogger::s_ClientLogger;

    bool CLogger::s_IsActive = false;

    void CLogger::Init()
    {
        if ( CLogger::s_IsActive )
            return;

#if defined( ENGINE_USE_LOGS )
        spdlog::set_pattern( "%^[%T] %n: %v%$" );
    #ifndef ENGINE_HEADLESS_EGL
        s_CoreLogger = spdlog::stdout_color_mt( "ENGINE" );
    #else
        s_CoreLogger = spdlog::stdout_color_mt( "ENGINE_EGL" );
    #endif /* ENGINE_HEADLESS_EGL */
        s_CoreLogger->set_level( spdlog::level::trace );

    #ifndef ENGINE_HEADLESS_EGL
        s_ClientLogger = spdlog::stdout_color_mt( "APP" );
    #else
        s_ClientLogger = spdlog::stdout_color_mt( "APP_EGL" );
    #endif
        s_ClientLogger->set_level( spdlog::level::trace );
#endif

        CLogger::s_IsActive = true;
    }

    void CLogger::Release()
    {
        // Clean refs. If using only macros for logging then all other references should
        // have been deleted once the macro finishes.
        s_CoreLogger = nullptr;
        s_ClientLogger = nullptr;
        s_IsActive = false;
        spdlog::drop_all();
    }

    bool CLogger::IsActive()
    {
        return CLogger::s_IsActive;
    }

}