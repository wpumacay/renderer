#pragma once

#include <CMacros.h>
#include <CExtensions.h>
#include <utils/CLogger.h>

#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <string>
#include <chrono>
#include <vector>

// Adapted from TheCherno's tutorial on profiling:
// video    : https://youtu.be/xlAH4dbMVnU
// gist     : https://gist.github.com/TheCherno/31f135eea6ee729ab5f26a6908eb3a5e

namespace engine
{

    struct CProfileResult
    {
        std::string name;
        long long   tstart;
        long long   tend;
        double      duration;
    };

    enum class eProfilerType
    {
        INTERNAL = 0,
        EXTERNAL_CHROME
    };

    enum class eProfilerState
    {
        IDLE = 0,
        RUNNING
    };

    /**
    *   @brief Scoped profiling timer
    *
    *   @details Profiling timer (using <chrono>) that keeps track of the amount of time a given
    *            scope took to execute. Use it along profiling sessions and the profiling manager,
    *            as this timer sends its results through the manager to the sessions to handle.
    *
    *   Example:
    *   @code
    *       // some scope we want to time
    *       {
    *           // Create a timer for the following scope
    *           engine::CProfilerTimer _ptimer( "scope-name", "sess_name" );
    *           // Alternatively, use the helper macros
    *           PROFILE_SCOPE( "scope-name", "sess_name" );;
    *       }
    *   @endcode
    */
    class CProfilerTimer
    {

    public :

        CProfilerTimer( const char* name, const char* session, bool verbose = false );

        ~CProfilerTimer();

    private :

        void _stop();

    private :

        const char* m_name;
        const char* m_session;
        bool m_stopped;
        bool m_verbose;

        std::chrono::time_point<std::chrono::high_resolution_clock> m_tpStart;

    };

    /**
    *   @brief Profiler Session Interface used to handle timers' profiling results
    *
    *   @details Profiling sessions are in charge of handling the results of the timers, either
    *            by storing them for later internal usage (eProfilerType::INTERNAL) or saving
    *            the results to a file for usage with chrome-tracing (eProfilerType::EXTERNAL_CHROME).
    *            Check the appropriate implementors of this interface for more details.
    *
    *   Example:
    *   @code
    *       // Create a profiling session (through profiling manager)
    *       engine::CProfilingManager::BeginSession( "sess_renderer" );
    *       // ...
    *       // Scope of some function to be profiled
    *       {
    *           PROFILE_FUNCTION_IN_SESSION( "sess_renderer" );
    *           // ... (function body)
    *       }
    *   @endcode
    *
    *   @see engine::CProfilingManager
    *   @see engine::eProfilerType
    */
    class CProfilerSession
    {

    public :

        CProfilerSession( const char* name ) : m_name( name ), m_state( eProfilerState::IDLE ) {}
        virtual ~CProfilerSession() {}

        virtual void begin() = 0;
        virtual void write( const CProfileResult& result ) = 0;
        virtual void end() = 0;

        eProfilerType type() const { return m_type; }
        eProfilerState state() const { return m_state; }
        const char* name() const { return m_name; }

    protected :

        const char* m_name;

        eProfilerType m_type;

        eProfilerState m_state;
    };

    /**
    *   @brief Profiler session for usage with internal logging tools
    *
    *   @details This profiler session stores the profile-results obtained by the profiling
    *            timers in its own buffer and exposes them such that internal logging tools
    *            can use them. Each time a session begins it clears the storage to accomodate
    *            for new incoming results, so the user has to grab the results before this happens.
    */
    class CProfilerSessionInternal : public CProfilerSession
    {

    public :

        CProfilerSessionInternal( const char* name ) : CProfilerSession( name ) { m_type = eProfilerType::INTERNAL; }
        ~CProfilerSessionInternal() {}

        void begin() override;
        void write( const CProfileResult& result ) override;
        void end() override;

        std::vector< CProfileResult > results() { return m_sessionResults; }

    private :

        std::vector< CProfileResult > m_sessionResults;
    };

    /**
    *   @brief Profiler session for usage with external chrome-tracing tool
    *
    *   @details This profiler session saves the profile results to disk in .json format, according
    *            to the required specs of the chrome-tracing tool. To see the results, just open a
    *            browser tab in chrome, navigate to chrome://tracing and load the .json file to see
    *            the logged results. Unlike
    */
    class CProfilerSessionExtChrome : public CProfilerSession
    {

    public :

        CProfilerSessionExtChrome( const char* name );
        ~CProfilerSessionExtChrome() {}

        void begin() override;
        void write( const CProfileResult& result ) override;
        void end() override;

    private :

        void _writeHeader();
        void _writeProfile();
        void _writeFooter();

    private :

        /** @brief File handle used to save the results to disk */
        std::ofstream m_fWriter;

        /** @brief Name of the json file saved to disk */
        std::string m_fName;
    };

    /**
    *   @brief Profiler Singleton with support for multiple sessions
    *
    *   @details This profiler singleton supports handling various sessions, each being
    *            responsible to store, save to disk, etc., the results of the profiling 
    *            timers. Sessions are cached to keep its contents through the profiler's
    *            lifetime, and reset during the begin-end calls, i.e. clearing internal
    *            containers, etc.
    *
    *   Example:
    *   @code
    *       // Initialize profiler
    *       engine::CProfilingManager::Init( eProfilerType::INTERNAL );
    *       // create a session for this whole block
    *       engine::CProfilingManager::BeginSession( "sess_renderer" );
    *       {
    *           // profile the time of this scope
    *           PROFILE_SCOPE( "shadow-pass", "sess_renderer" );
    *       }
    *       // notify session that all required profiling events have completed
    *       engine::CProfilingManager::EndSession( "sess_renderer" );
    *   @endcode
    */
    class CProfilingManager
    {

    public :

        static void Init( const eProfilerType& type = eProfilerType::INTERNAL );
        static void Release();
        static void BeginSession( const char* session );
        static void EndSession( const char* session );
        static void WriteProfileResult( const CProfileResult& result, const char* session = "sess_default" );

        ~CProfilingManager() {}

        static std::vector< CProfilerSession* > GetSessions();

    protected :

        CProfilingManager( const eProfilerType& type ) : m_profilerType( type ) {}

        void _beginSession( const char* session );
        void _endSession( const char* session );
        void _writeProfileResult( const CProfileResult& result, const char* session );

        std::vector< CProfilerSession* > _getSessions();

    private :

        /** @brief cache of session created during manager's lifetime */
        std::unordered_map< std::string, std::unique_ptr<CProfilerSession> > m_sessions;

        /** @brief type of profiling-sessions to use */
        eProfilerType m_profilerType;

        static CProfilingManager* _instance;

    };

}

#define PROFILE_SCOPE(name) engine::CProfilerTimer profTimer##__LINE__(name, "sess_default")
#define PROFILE_SCOPE_IN_SESSION(name, sessionName) engine::CProfilerTimer profTimer##__LINE__(name, sessionName)

#define PROFILE_FUNCTION() engine::CProfilerTimer profTimer##__LINE__(__FUNCTION_NAME__, "sess_default")
#define PROFILE_FUNCTION_IN_SESSION(sessionName) engine::CProfilerTimer profTimer##__LINE__(__FUNCTION_NAME__, sessionName)