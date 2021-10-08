#pragma once

#include <CCommon.h>
#include <CMath.h>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace engine
{

    struct CImGuiProps
    {
        bool useDockingSpace;
        bool useDockingSpacePassthrough;
        bool useAutosaveLayout;
        std::string fileLayout;

        CImGuiProps()
        {
            useDockingSpace = false;
            useDockingSpacePassthrough = false;
            useAutosaveLayout = true;
            fileLayout = "";
        }
    };

    class CImGuiManager
    {

    public :

        CImGuiManager( GLFWwindow* glfwWindowPtr,
                       const CImGuiProps& props = CImGuiProps() );
        ~CImGuiManager();

        void begin();
        void render();

        void setActive( bool active ) { m_active = active; }
        void setDockingSpace( bool enabled );
        void setDockingSpacePassthrough( bool enabled );
        bool active() const { return m_active; }
        bool usesDockingSpace() const { return m_properties.useDockingSpace; }
        bool usesDockingSpacePassthrough() const { return m_properties.useDockingSpacePassthrough; }

    private :

        void _configureDockingSpace();

    private :

        bool m_active;
        CImGuiProps m_properties;

        ImGuiDockNodeFlags  m_dockSpaceFlags;       // docking configuration options
        ImGuiWindowFlags    m_dockSpaceWindowFlags; // central-window (docking node) configuration

        /* handle to glfw window to copy (or create more, if viewports enabled???) contexts */
        GLFWwindow* m_glfwWindowPtr;
    };

}
