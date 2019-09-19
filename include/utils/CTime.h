#pragma once

#include <utils/CLogger.h>

namespace engine
{

    class CTime
    {
    public :

        static void Init();
        static void Release();
        static void Start( float maxDeltaTime = 0.05 );
        static void Update( float timestep );
        static float GetTimeStep();
        static float GetRawTimeStep();
        static float GetWallTime();

        ~CTime();

    private :

        CTime();

        void _start( float maxDeltaTime );
        void _update( float timestep );

    private :
        static CTime* _instance;

        float m_maxDeltaTime;

        float m_timeCurrent;
        float m_timeDelta;
        float m_timeDeltaRaw;
    };

}