

#pragma once

#include "../LCommon.h"
#include "../LConfig.h"
#include "../window/LWindow.h"

namespace engine 
{ 

    class LBaseApp
    {

        protected :

        LWindow* m_window;

        float m_timeBef;
        float m_timeNow;
        float m_timeDelta;

        public :

        LBaseApp();
        ~LBaseApp();

        void loop();
        virtual void render();

        inline float timeDelta() { return m_timeDelta; }
    };

}