#pragma once

#include <LScene.h>

#include <camera/CICamera.h>
#include <camera/CFpsCamera.h>
#include <camera/CFixedCamera.h>

#include <LMasterRenderer.h>

#include <LAssetsManager.h>
#include <shaders/LShaderManager.h>

#include <debug/LDebugSystem.h>
#include <input/LInputSystem.h>

#include <core/COpenGLWindow.h>
#include <utils/CLogger.h>
#include <utils/CTime.h>
#include <utils/CImguiUi.h>

namespace engine
{

    class COpenGLApp
    {
    public :
        static COpenGLApp* GetInstance();
        static COpenGLWindow* GetWindow();

        COpenGLApp();
        virtual ~COpenGLApp();

        void init();

        void begin();

        void update();

        void end();

        LScene* scene();

        bool isActive();

        float frametime();

        float fps();

    protected :

        virtual void _initUser();
        virtual void _updateUser();

    protected :

        static COpenGLApp* s_instance;

        LMasterRenderer* m_masterRenderer;
        LScene* m_scenePtr;

        COpenGLWindow* m_windowPtr;
        CImguiUi* m_uiPtr;

        float m_timeNow;
        float m_timeDelta;
    };

}