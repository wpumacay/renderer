#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>
#include <gl/COpenGLContext.h>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace engine
{
    class CImguiUi
    {
    public :
        CImguiUi( COpenGLContext* context );
        virtual ~CImguiUi();

        void init();
        void render();

    protected :
        virtual void _initInternal() = 0;
        virtual void _renderInternal() = 0;

    protected :
        GLFWwindow* m_windowHandle;
    };
}