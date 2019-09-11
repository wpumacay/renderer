
#include <utils/CLogger.h>

namespace engine
{

    std::shared_ptr<spdlog::logger> CLogger::s_CoreLogger;
    std::shared_ptr<spdlog::logger> CLogger::s_ClientLogger;

    void CLogger::Init()
    {
        spdlog::set_pattern( "%^[%T] %n: %v%$" );

        s_CoreLogger = spdlog::stdout_color_mt( "ENGINE" );
        s_CoreLogger->set_level( spdlog::level::trace );

        s_ClientLogger = spdlog::stdout_color_mt( "APP" );
        s_ClientLogger->set_level( spdlog::level::trace );
    }

}