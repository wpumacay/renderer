
#include <utils/CTime.h>

namespace engine
{

    CTime* CTime::_instance = nullptr;

    CTime::CTime()
    {
        m_maxDeltaTime = 0.05f;
        m_timeCurrent = 0.0f;
        m_timeDelta = 0.05f;
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

    float CTime::GetWallTime()
    {
        ENGINE_CORE_ASSERT( CTime::_instance, "Should have initialized time keeper before using it" );

        return CTime::_instance->m_timeCurrent;
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
    }

}