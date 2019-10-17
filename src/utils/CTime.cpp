
#include <utils/CTime.h>

namespace engine
{

    CTime* CTime::_instance = nullptr;

    CTime::CTime()
    {
        m_maxDeltaTime = 0.05f;
        m_timeCurrent = 0.0f;
        m_timeDelta = 0.05f;
        m_timeDeltaAvg = 0.0f;
        m_timeDeltaRaw = 0.0f;

        m_frameTimeIndex = 0;
        for ( size_t i = 0; i < TIME_NUM_FRAMES_TO_AVERAGE; i++ )
            m_frameTimes[i] = 0.0f;
        for ( size_t i = 0; i < TIME_NUM_FRAMES_TO_AVERAGE; i++ )
            m_fpsTimesAvg[i] = 0.0f;
    }

    CTime::~CTime()
    {
        // nothing to do here
    }

    void CTime::Init()
    {
        if ( CTime::_instance )
        {
            ENGINE_CORE_WARN( "Tried to initialize time keeper again" );
            return;
        }

        CTime::_instance = new CTime();
    }

    void CTime::Release()
    {
        if ( CTime::_instance )
            delete CTime::_instance;

        CTime::_instance = nullptr;
    }

    void CTime::Start( float maxDeltaTime )
    {
        ENGINE_CORE_ASSERT( CTime::_instance, "Should have initialized time keeper before using it" );

        CTime::_instance->_start( maxDeltaTime );
    }

    void CTime::Update( float timestep )
    {
        ENGINE_CORE_ASSERT( CTime::_instance, "Should have initialized time keeper before using it" );

        CTime::_instance->_update( timestep );
    }

    float CTime::GetTimeStep()
    {
        ENGINE_CORE_ASSERT( CTime::_instance, "Should have initialized time keeper before using it" );

        return CTime::_instance->m_timeDelta;
    }

    float CTime::GetRawTimeStep()
    {
        ENGINE_CORE_ASSERT( CTime::_instance, "Should have initialized time keeper before using it" );

        return CTime::_instance->m_timeDeltaRaw;
    }

    float CTime::GetAvgTimeStep()
    {
        ENGINE_CORE_ASSERT( CTime::_instance, "Should have initialized time keeper before using it" );

        return CTime::_instance->m_timeDeltaAvg;
    }

    float CTime::GetWallTime()
    {
        ENGINE_CORE_ASSERT( CTime::_instance, "Should have initialized time keeper before using it" );

        return CTime::_instance->m_timeCurrent;
    }

    float* CTime::GetFrameTimes()
    {
        ENGINE_CORE_ASSERT( CTime::_instance, "Should have initialized time keeper before using it" );

        return CTime::_instance->m_frameTimes;
    }

    float* CTime::GetFpsAvgs()
    {
        ENGINE_CORE_ASSERT( CTime::_instance, "Should have initialized time keeper before using it" );

        return CTime::_instance->m_fpsTimesAvg;
    }

    int CTime::GetNumFramesForAvg()
    {
        ENGINE_CORE_ASSERT( CTime::_instance, "Should have initialized time keeper before using it" );

        return TIME_NUM_FRAMES_TO_AVERAGE;
    }

    int CTime::GetFrameTimeIndex()
    {
        ENGINE_CORE_ASSERT( CTime::_instance, "Should have initialized time keeper before using it" );

        return CTime::_instance->m_frameTimeIndex;
    }

    void CTime::_start( float maxDeltaTime )
    {
        m_maxDeltaTime = maxDeltaTime;
        m_timeCurrent = 0.0f;
        m_timeDelta = maxDeltaTime;
    }

    void CTime::_update( float timestep )
    {
        m_timeDeltaRaw = timestep;
        m_timeCurrent += timestep;

        m_timeDelta = std::min( timestep, m_maxDeltaTime );

        // compute avg-time step over averaging window
        m_timeDeltaAvg = m_timeDeltaAvg + ( timestep - m_frameTimes[m_frameTimeIndex] ) / TIME_NUM_FRAMES_TO_AVERAGE;
        m_frameTimes[m_frameTimeIndex] = timestep;
        m_fpsTimesAvg[m_frameTimeIndex] = 1.0f / m_timeDeltaAvg;
        m_frameTimeIndex = ( m_frameTimeIndex + 1 ) % TIME_NUM_FRAMES_TO_AVERAGE;
    }

}