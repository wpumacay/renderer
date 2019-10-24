#pragma once

#include <graphics/CScene.h>

#include <camera/CICamera.h>
#include <camera/CFixedCamera.h>
#include <camera/CFpsCamera.h>
#include <camera/COrbitCamera.h>

#include <renderers/CMainRenderer.h>

#include <assets/CTextureManager.h>
#include <shaders/CShaderManager.h>
#include <input/CInputManager.h>
#include <input/CInputEvent.h>

#include <gl/COpenGLWindow.h>

#include <utils/CLogger.h>
#include <utils/CTime.h>
#include <utils/CDebugDrawer.h>
#include <utils/CNoiseGenerator.h>

#include <gui/CImGuiManager.h>
#include <gui/CImGuiLayer.h>

namespace engine
{

    class CApplication
    {

    public :

        static CApplication* GetInstance();

        CApplication();
        virtual ~CApplication();

        void setScene( std::unique_ptr< CScene > scene );

        void addGuiLayer( std::unique_ptr< CImGuiLayer > layer );

        void update();

        void begin();

        void render();

        void end();

        static void CallbackKey( int key, int action );
        static void CallbackMouseButton( int button, int action, double x, double y );
        static void CallbackMouseMove( double x, double y );
        static void CallbackScroll( double xOff, double yOff );
        static void CallbackResize( int width, int height );

        std::vector< CImGuiLayer* > guiLayers() const;

        CRenderOptions& renderOptions() { return m_renderOptions; }

        CScene* scene() const { return m_scene.get(); }

        COpenGLWindow* window() const { return m_window.get(); }

        CMainRenderer* renderer() const { return m_mainRenderer.get(); }

        bool active() const { return m_window->active(); }

    protected :

        static CApplication* s_instance;

        float32                                         m_timeStamp;
        CRenderOptions                                  m_renderOptions;
        std::unique_ptr< CScene >                       m_scene;
        std::unique_ptr< COpenGLWindow >                m_window;
        std::unique_ptr< CMainRenderer >                m_mainRenderer;
        std::unique_ptr< CImGuiManager >                m_imguiManager;
        std::vector< std::unique_ptr< CImGuiLayer > >   m_guiLayers;
    };

}