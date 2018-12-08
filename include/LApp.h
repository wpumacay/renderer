
#pragma once

#include "LScene.h"
#include "LFpsCamera.h"
#include "LFixedCamera3d.h"
#include "LMasterRenderer.h"
#include "LAssetsManager.h"
#include "shaders/LShaderManager.h"
#include "debug/LDebugSystem.h"
#include "input/LInputSystem.h"

namespace engine
{

    class LApp
    {
        private :

        static LApp* _INSTANCE;
        LApp();

        LMasterRenderer* m_masterRenderer;
        LScene* m_scene;
        LWindow* m_window;

        float m_tBef;
        float m_tNow;
        float m_tDelta;

        public :

        static LApp* GetInstance();
        ~LApp();

        void begin();
        void update();
        void end();
        LScene* scene();
        bool isActive();

        LWindow* window();
    };

}