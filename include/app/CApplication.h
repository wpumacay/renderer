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
#include <gui/CImGuiUtilsLayer.h>
#include <gui/CImGuiSceneLayer.h>

namespace engine
{

    class CApplication
    {

    public :

        static CApplication* GetInstance();

        CApplication( const CWindowProps& windowProperties = CWindowProps() );
        virtual ~CApplication();

        void setScene( std::unique_ptr< CScene > scene );

        void setOffscreenRendering( bool enabled );

        void setGuiActive( bool enabled );

        void setGuiUtilsActive( bool enabled );

        void setGuiSceneViewActive( bool enabled );

        void addGuiLayer( std::unique_ptr< CImGuiLayer > layer );

        void update();

        void begin();

        void render();

        void end();

        void addKeyboardCallback( FnPtr_keyboard_callback callback ) { m_keyboardCallbacks.push_back( callback ); }
        void addMouseButtonCallback( FnPtr_mouse_callback callback ) { m_mouseButtonCallbacks.push_back( callback ); }
        void addMouseMoveCallback( FnPtr_mousemove_callback callback ) { m_mouseMoveCallbacks.push_back( callback ); }
        void addScrollCallback( FnPtr_scroll_callback callback ) { m_scrollCallbacks.push_back( callback ); }
        void addResizeCallback( FnPtr_resize_callback callback ) { m_resizeCallbacks.push_back( callback ); }

        static void CallbackKey( int key, int action );
        static void CallbackMouseButton( int button, int action, double x, double y );
        static void CallbackMouseMove( double x, double y );
        static void CallbackScroll( double xOff, double yOff );
        static void CallbackResize( int width, int height );

        std::vector< CImGuiLayer* > guiLayers() const;

        CRenderOptions& renderOptions() { return m_renderOptions; }

        CFrameBuffer* renderTarget() const { return m_renderTarget.get(); }

        CScene* scene() const { return m_scene.get(); }

        COpenGLWindow* window() const { return m_window.get(); }

        CMainRenderer* renderer() const { return m_mainRenderer.get(); }

        CImGuiManager* imguiManager() const { return m_imguiManager.get(); }

        bool active() const { return m_window->active(); }

        bool guiActive() const { return m_imguiManager->active(); }

        bool guiUtilsActive() const { return m_guiUtilsLayer->active(); }

        bool guiSceneViewActive() const { return false; }

    protected :

        CFrameBuffer* _createRenderTarget();

        static CApplication* s_instance;

        float32                                         m_timeStamp;
        CRenderOptions                                  m_renderOptions;
        std::unique_ptr< CFrameBuffer >                 m_renderTarget;
        std::unique_ptr< CScene >                       m_scene;
        std::unique_ptr< COpenGLWindow >                m_window;
        std::unique_ptr< CMainRenderer >                m_mainRenderer;
        std::unique_ptr< CImGuiManager >                m_imguiManager;
        std::vector< std::unique_ptr< CImGuiLayer > >   m_guiLayers;

        CImGuiUtilsLayer* m_guiUtilsLayer;

        bool m_useRenderTarget;

        std::vector< FnPtr_keyboard_callback > m_keyboardCallbacks;
        std::vector< FnPtr_mouse_callback > m_mouseButtonCallbacks;
        std::vector< FnPtr_mousemove_callback > m_mouseMoveCallbacks;
        std::vector< FnPtr_scroll_callback > m_scrollCallbacks;
        std::vector< FnPtr_resize_callback > m_resizeCallbacks;
    };

}