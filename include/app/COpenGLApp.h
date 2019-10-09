#pragma once

#include <graphics/CScene.h>

#include <camera/CICamera.h>
#include <camera/CFixedCamera.h>
#include <camera/CFpsCamera.h>
#include <camera/COrbitCamera.h>

// #include <renderers/CMainRenderer.h>

#include <assets/CTextureManager.h>
#include <shaders/CShaderManager.h>

#include <input/CInputHandler.h>

#include <assets/CTextureManager.h>
#include <app/COpenGLWindow.h>

#include <utils/CLogger.h>
#include <utils/CTime.h>
#include <utils/CImguiUi.h>
#include <utils/CDebugDrawer.h>
#include <utils/CNoiseGenerator.h>

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

        void update();

        void beginRendering();

        void renderScene();

        void renderUi();

        void renderDebug();

        void endRendering();

        void setScene( std::unique_ptr< CScene > scenePtr ) { m_scenePtr = std::move( scenePtr ); }

        void setUi( std::unique_ptr< CImguiUi > uiPtr ) { m_uiPtr = std::move( uiPtr ); }

        CScene* scene() const { return m_scenePtr.get(); }

        CImguiUi* ui() const { return m_uiPtr.get(); }

        COpenGLWindow* window() const { return m_windowPtr.get(); }

        // CMainRenderer* renderer() const { return m_mainRenderer.get(); }

        bool active() const { return m_windowPtr->active(); }

        float frametime() const { return m_timeDelta; }

        float fps() const { return 1.0f / m_timeDelta; }

    protected :

        virtual void _initUser();
        virtual void _updateUser();

    protected :

        static COpenGLApp* s_instance;

        std::unique_ptr< CScene >           m_scenePtr;
        std::unique_ptr< CImguiUi >         m_uiPtr;
        std::unique_ptr< COpenGLWindow >    m_windowPtr;
        // std::unique_ptr< CMainRenderer>     m_mainRendererPtr;

        float32 m_timeNow;
        float32 m_timeDelta;
    };

}