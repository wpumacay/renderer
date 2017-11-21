

#include <core/LBaseApp.h>

#include <iostream>

using namespace std;


namespace engine
{
    namespace core
    {

        LBaseApp* LBaseApp::instance = NULL;

        LBaseApp::LBaseApp()
        {
#ifdef USE_GLFW
            m_window = NULL;
            m_initialized = false;
#endif
        }

        LBaseApp::~LBaseApp()
        {
#ifdef USE_GLFW
            m_window = NULL;
#endif
        }

        void LBaseApp::create()
        {
            if ( LBaseApp::instance != NULL )
            {
                delete LBaseApp::instance;
            }

            LBaseApp::instance = new LBaseApp();
            LBaseApp::instance->init();
        }

        void LBaseApp::destroy()
        {
            LBaseApp::instance->finalize();
            delete LBaseApp::instance;
            LBaseApp::instance = NULL;
        }


        bool LBaseApp::init()
        {
        #ifdef USE_GLFW
            glfwInit();
            glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, ENGINE_GL_CONTEXT_VERSION_MAJOR );
            glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, ENGINE_GL_CONTEXT_VERSION_MINOR );
            glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE );
            glfwWindowHint( GLFW_RESIZABLE, ENGINE_WINDOW_RESIZABLE );

            m_window = glfwCreateWindow( ENGINE_APP_WIDTH, 
                                         ENGINE_APP_HEIGHT,
                                         ENGINE_APP_NAME,
                                         NULL, NULL );

            if ( m_window == NULL )
            {
                glfwTerminate();
                ENGINE_ERROR( "LBaseApp::init> could not initialize GLFW" );
                return false;
            }

            glfwMakeContextCurrent( m_window );

            // Initialize glew
            glewExperimental = GL_TRUE;
            if ( glewInit() != GLEW_OK )
            {
                glfwTerminate();
                ENGINE_ERROR( "LBaseApp::init> could not initialize GLEW" );
                return false;
            }

            glfwSetKeyCallback( m_window, LBaseApp::onKeyEvent );
            glfwSetMouseButtonCallback( m_window, LBaseApp::onMouseEvent );
            glfwSetCursorPosCallback( m_window, LBaseApp::onCursorPosEvent );
            glfwSetScrollCallback( m_window, LBaseApp::onScrollEvent );

            glfwGetFramebufferSize( m_window, &m_width, &m_height );
            glViewport( 0, 0, m_width, m_height );

        #elif USE_GLUT

            int _argc = 0;
            char* _argv;

            glutInit( &_argc, &_argv );
            glutInitWindowSize( ENGINE_APP_WIDTH, ENGINE_APP_HEIGHT );
            glutInitDisplayMode( GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH );
            glutCreateWindow( ENGINE_APP_NAME );

            glewExperimental = GL_TRUE;
            if ( glewInit() != GLEW_OK )
            {
                ENGINE_ERROR( "LBaseApp::init> could not initialize GLEW" );
                return false;
            }

            glutDisplayFunc( LBaseApp::onDisplayEvent );
            glutMouseFunc( LBaseApp::onMouseEvent );
            glutMotionFunc( LBaseApp::onCursorPosEvent );
            glutKeyboardFunc( LBaseApp::onKeyDownEvent );
            glutKeyboardUpFunc( LBaseApp::onKeyUpEvent );
            glutIdleFunc( LBaseApp::onIdleEvent );

        #else

            ENGINE_ERROR( "LBaseApp::init> wrong configuration of window system" );

        #endif

            m_initialized = true;

            onStartup();

            return true;
        }

        void LBaseApp::loop()
        {
#ifdef USE_GLFW
            if ( !m_initialized )
            {
                return;
            }

            while ( !glfwWindowShouldClose( m_window ) )
            {
                glfwPollEvents();
                        
                m_timeNow = glfwGetTime();
                m_timeDelta = m_timeNow - m_timeBef;
                m_timeBef = m_timeNow;

                render();

                glfwSwapBuffers( m_window );
            }
#else
            glutMainLoop();
#endif
        }

        void LBaseApp::finalize()
        {
#ifdef USE_GLFW
            glfwTerminate();
#endif
        }

#ifdef USE_GLFW

        void LBaseApp::onKeyEvent( GLFWwindow* pWindow, int pKey, int pScancode, 
                                   int pAction, int pMode )
        {
            LBaseApp::instance->onKeyCallback( pKey, pScancode, pAction, pMode );
        }

        void LBaseApp::onMouseEvent( GLFWwindow* pWindow, int pButton, 
                                     int pAction, int pMods )
        {
            double evx, evy;

            glfwGetCursorPos( pWindow, &evx, &evy );

            LBaseApp::instance->onMouseButtonCallback( pButton, pAction, pMods );
        }

        void LBaseApp::onCursorPosEvent( GLFWwindow* pWindow, double x, double y )
        {
            LBaseApp::instance->onCursorCallback( x, y );
        }

        void LBaseApp::onScrollEvent( GLFWwindow* pWindow, double xOff, double yOff )
        {
            LBaseApp::instance->onMouseScrollCallback( xOff, yOff );
        }

#else


        void LBaseApp::onKeyDownEvent( unsigned char pKey, int x, int y )
        {
            LBaseApp::instance->onKeyCallback( (int) pKey, KEY_DOWN );
        }

        void LBaseApp::onKeyUpEvent( unsigned char pKey, int x, int y )
        {
            LBaseApp::instance->onKeyCallback( (int) pKey, KEY_UP );
        }

        void LBaseApp::onMouseEvent( int pButton, 
                                     int pAction, 
                                     int x, int y )
        {
            LBaseApp::instance->onMouseButtonCallback( pButton, pAction, x, y );
        }

        void LBaseApp::onCursorPosEvent( int x, int y )
        {
            LBaseApp::instance->onCursorCallback( x, y );
        }        

        void LBaseApp::onDisplayEvent()
        {
            LBaseApp::instance->render();

            glutSwapBuffers();
        }

        void LBaseApp::onIdleEvent()
        {
            glutPostRedisplay();
        }


#endif

    }   
}