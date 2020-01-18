
#include <utils/CProfiling.h>

namespace engine
{

    /**********************************************************************************************/
    /*                                 Scoped Profiling Timer                                     */
    /**********************************************************************************************/

    CProfilerTimer::CProfilerTimer( const char* name, const char* session, bool verbose )
    {
        m_name = name;
        m_session = session;
        m_tpStart = std::chrono::high_resolution_clock::now();
        m_stopped = false;
        m_verbose = verbose;
    }

    CProfilerTimer::~CProfilerTimer()
    {
        if ( !m_stopped )
            _stop();
    }

    void CProfilerTimer::_stop()
    {
        auto _tpEnd = std::chrono::high_resolution_clock::now();

        long long _tStart = std::chrono::time_point_cast<std::chrono::microseconds>( m_tpStart ).time_since_epoch().count();
        long long _tEnd = std::chrono::time_point_cast<std::chrono::microseconds>( _tpEnd ).time_since_epoch().count();

        double _duration = (_tEnd - _tStart) * 0.001;

        CProfilingManager::WriteProfileResult( { m_name, _tStart, _tEnd, _duration }, m_session );

        m_stopped = true;
    }

    /**********************************************************************************************/
    /*                                 Internal profiling session                                 */
    /**********************************************************************************************/

    CProfilerSessionExtChrome::CProfilerSessionExtChrome( const char* name )
        : CProfilerSession( name )
    {
        m_type = eProfilerType::EXTERNAL_CHROME;
        m_fName = std::string( m_name ) + ".json";
    }

    void CProfilerSessionInternal::begin()
    {
        // clear all results from previous logging pass
        m_sessionResults.clear();
        m_state = eProfilerState::RUNNING;
    }

    void CProfilerSessionInternal::write( const CProfileResult& result )
    {
        m_sessionResults.push_back( result );
    }

    void CProfilerSessionInternal::end()
    {
        m_state = eProfilerState::IDLE;
    }

    /**********************************************************************************************/
    /*                              Chrome-tracing profiling session                              */
    /**********************************************************************************************/

    void CProfilerSessionExtChrome::begin()
    {
        // open a file without append for each begin-session request
        m_fWriter.open( m_fName, std::ofstream::out );
        if ( !m_fWriter.is_open() )
        {
            ENGINE_CORE_WARN( "CProfilerSessionExtChrome::begin() >>> couldn't open session file {0}", m_name );
            return;
        }

        _writeHeader();
        m_state = eProfilerState::RUNNING;
    }

    void CProfilerSessionExtChrome::write( const CProfileResult& result )
    {
        if ( m_state != eProfilerState::RUNNING )
            return;

        std::stringstream json;

        std::string name = result.name;
        std::replace( name.begin(), name.end(), '"', '\'' );

        json << ",{";
        json << "\"cat\":\"function\",";
        json << "\"dur\":" << (result.tend - result.tstart) << ',';
        json << "\"name\":\"" << name << "\",";
        json << "\"ph\":\"X\",";
        json << "\"pid\":0,";
        json << "\"tid\":0,";
        json << "\"ts\":" << result.tstart;
        json << "}";

        m_fWriter << json.str();
        m_fWriter.flush();
    }

    void CProfilerSessionExtChrome::end()
    {
        if ( m_state != eProfilerState::RUNNING )
            return;

        _writeFooter();
        m_fWriter.close();
        m_state = eProfilerState::IDLE;
    }

    void CProfilerSessionExtChrome::_writeHeader()
    {
        m_fWriter << "{\"otherData\": {},\"traceEvents\":[{}";
        m_fWriter.flush();
    }

    void CProfilerSessionExtChrome::_writeFooter()
    {
        m_fWriter << "]}";
        m_fWriter.flush();
    }

    /**********************************************************************************************/
    /*                                    Profiling manager                                       */
    /**********************************************************************************************/

    CProfilingManager* CProfilingManager::_instance = nullptr;

    void CProfilingManager::Init( const eProfilerType& type )
    {
        if ( !CProfilingManager::_instance )
            CProfilingManager::_instance = new CProfilingManager( type );

        //// // create default session
        //// CProfilingManager::_instance->_beginSession( "sess_default" );
    }

    void CProfilingManager::Release()
    {
        if ( !CProfilingManager::_instance )
            return;

        delete CProfilingManager::_instance;
        CProfilingManager::_instance = nullptr;
    }

    void CProfilingManager::BeginSession( const char* session )
    {
        if ( CProfilingManager::_instance )
            CProfilingManager::_instance->_beginSession( session );
    }

    void CProfilingManager::EndSession( const char* session )
    {
        if ( CProfilingManager::_instance )
            CProfilingManager::_instance->_endSession( session );
    }

    void CProfilingManager::WriteProfileResult( const CProfileResult& result, const char* session )
    {
        if ( CProfilingManager::_instance )
            CProfilingManager::_instance->_writeProfileResult( result, session );
    }

    std::vector< CProfilerSession* > CProfilingManager::GetSessions()
    {
        if ( CProfilingManager::_instance )
            return CProfilingManager::_instance->_getSessions();

        return {};
    }

    void CProfilingManager::_beginSession( const char* session )
    {
        if ( m_sessions.find( session ) == m_sessions.end() )
        {
            if ( m_profilerType == eProfilerType::INTERNAL )
                m_sessions[session] = std::make_unique<CProfilerSessionInternal>( session );
            else
                m_sessions[session] = std::make_unique<CProfilerSessionExtChrome>( session );
        }

        m_sessions[session]->begin();
    }

    void CProfilingManager::_endSession( const char* session )
    {
        if ( m_sessions.find( session ) == m_sessions.end() )
            ENGINE_CORE_WARN( "CProfilingManager::_endSession() >>> session with name {0} not found", session );
        else
            m_sessions[session]->end();
    }

    void CProfilingManager::_writeProfileResult( const CProfileResult& result, const char* session )
    {
        if ( m_sessions.find( session ) == m_sessions.end() )
            ENGINE_CORE_WARN( "CProfilingManager::_writeProfileResult() >>> session with name {0} not found", session );
        else
            m_sessions[session]->write( result );
    }

    std::vector< CProfilerSession* > CProfilingManager::_getSessions()
    {
        std::vector< CProfilerSession* > _vsessions;
        for ( auto& kvpair : m_sessions )
            _vsessions.push_back( kvpair.second.get() );
        return _vsessions;
    }

}