
#include <utils/CLogger.h>

namespace engine
{

    std::shared_ptr<spdlog::logger> CLogger::s_CoreLogger;
    std::shared_ptr<spdlog::logger> CLogger::s_ClientLogger;

    bool CLogger::s_Initialized = false;

    void CLogger::Init()
    {
        if ( CLogger::s_Initialized )
            return;

#if defined( ENGINE_USE_LOGS )
        spdlog::set_pattern( "%^[%T] %n: %v%$" );

        s_CoreLogger = spdlog::stdout_color_mt( "ENGINE" );
        s_CoreLogger->set_level( spdlog::level::trace );

        s_ClientLogger = spdlog::stdout_color_mt( "APP" );
        s_ClientLogger->set_level( spdlog::level::trace );
#endif

        CLogger::s_Initialized = true;
    }

    void CLogger::Release()
    {
        // Clean refs. If using only macros for logging then all other references should
        // have been deleted once the macro finishes.
        s_CoreLogger = nullptr;
        s_ClientLogger = nullptr;
        s_Initialized = false;
    }

}