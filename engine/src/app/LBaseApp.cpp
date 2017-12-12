

#include "../../include/app/LBaseApp.h"

using namespace std;

namespace engine
{
    LBaseApp::LBaseApp()
    {
        m_window = new LWindow();
    }

    LBaseApp::~LBaseApp()
    {
        delete m_window;
    }

    void LBaseApp::loop()
    {
        while ( m_window->isActive() )
        {
            m_window->pollEvents();
                        
            m_timeNow = glfwGetTime();
            m_timeDelta = m_timeNow - m_timeBef;
            m_timeBef = m_timeNow;

            render();

            m_window->swapBuffers();
        }
    }

    void LBaseApp::render()
    {
        // Override this
    }

}