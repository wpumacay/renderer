#pragma once

#include <utils/CLogger.h>

#define TIME_NUM_FRAMES_TO_AVERAGE 100

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
        static float GetAvgTimeStep();
        static float GetAvgFps();
        static float GetWallTime();
        static float* GetFrameTimes();
        static float* GetFpsAvgs();
        static int GetNumFramesForAvg();
        static int GetFrameTimeIndex();

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
        float m_timeDeltaAvg;
        float m_timeDeltaRaw;

        float m_frameTimes[TIME_NUM_FRAMES_TO_AVERAGE];
        float m_fpsTimesAvg[TIME_NUM_FRAMES_TO_AVERAGE];
        int m_frameTimeIndex;
    };

}