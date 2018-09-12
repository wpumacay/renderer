
#pragma once

#include "LScene.h"
#include "LFpsCamera.h"
#include "LInputHandler.h"
#include "shaders/LShaderManager.h"
#include "LMasterRenderer.h"

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

        void update();
        LScene* scene();
        bool isActive();
    };



}