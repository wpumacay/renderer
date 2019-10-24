#pragma once

#include <CCommon.h>
#include <CMath.h>
#include <utils/CLogger.h>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace engine
{

    class CImGuiManager
    {

    public :

        CImGuiManager( GLFWwindow* glfwWindowPtr );
        ~CImGuiManager();

        void begin();
        void render();

        void setActive( bool active ) { m_active = active; }
        bool active() const { return m_active; }

    private :

        /* whether or not create ui render-frame */
        bool m_active;

        /* handle to glfw window to copy (or create more, if viewports enabled???) contexts */
        GLFWwindow* m_glfwWindowPtr;
    };

}