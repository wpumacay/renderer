
#include <window/CWindowEGL.h>

namespace engine
{
    CWindowEGL::CWindowEGL( const CWindowProps& properties )
        : CIWindow( properties )

    {
        m_Type = eWindowType::HEADLESS_EGL;
        // Load EGL-functions using glad (first load without display)
        int eglVersion = gladLoaderLoadEGL( NULL );
        ENGINE_CORE_ASSERT( eglVersion, "CWindowEGL >>> something went wrong during first-load of EGL functions using GLAD (with no display)" );

        // Initialize EGL
        m_EGLDisplay = eglGetDisplay( EGL_DEFAULT_DISPLAY );
        EGLint major, minor;
        ENGINE_CORE_ASSERT( eglInitialize( m_EGLDisplay, &major, &minor ), "CWindowEGL >>> unable to initialize EGL" );
        ENGINE_CORE_TRACE( "CWindowEGL >>> initialized EGL display, major: {0}, minor: {1}", major, minor );

        // Reload EGL-functions using glad (now with a properly initliazed display)
        eglVersion = gladLoaderLoadEGL( m_EGLDisplay );
        ENGINE_CORE_ASSERT( eglVersion, "CWindowEGL >>> something went wrong during reload of EGL functions using GLAD (with a display)" );

        // Select an appropriate configuration
        EGLint numConfigs;
        const EGLint configAttributes[] = {
                    EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
                    EGL_BLUE_SIZE, 8,
                    EGL_GREEN_SIZE, 8,
                    EGL_RED_SIZE, 8,
                    EGL_DEPTH_SIZE, 8,
                    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
                    EGL_NONE
        };
        const EGLint pbufferAttributes[] = {
            EGL_WIDTH, m_Properties.width,
            EGL_HEIGHT, m_Properties.height,
            EGL_NONE
        };

        eglChooseConfig( m_EGLDisplay, configAttributes, &m_EGLConfig, 1, &numConfigs );

        // Create an EGL surface
        m_EGLSurface = eglCreatePbufferSurface( m_EGLDisplay, m_EGLConfig,
                                                pbufferAttributes );

        // Bind the API
        eglBindAPI( EGL_OPENGL_API );

        // Create a context and make it current
        m_EGLContext = eglCreateContext( m_EGLDisplay, // current display
                                         m_EGLConfig, // current configuration
                                         EGL_NO_CONTEXT, // don't share context
                                         NULL ); // no extra attributes
        eglMakeCurrent( m_EGLDisplay, m_EGLSurface, m_EGLSurface, m_EGLContext );

        // Load gl-functions using glad
        ENGINE_CORE_ASSERT( gladLoadGL( (GLADloadfunc) eglGetProcAddress ), "CWindowEGL >>> failed to load GL using GLAD on the current EGL context" );
        ENGINE_CORE_INFO( "CWindowEGL >>> successfully initialized EGL window" );
        ENGINE_CORE_INFO( "OpenGL Info:" );
        ENGINE_CORE_INFO( "\tVendor     : {0}", glGetString( GL_VENDOR ) );
        ENGINE_CORE_INFO( "\tRenderer   : {0}", glGetString( GL_RENDERER ) );
        ENGINE_CORE_INFO( "\tVersion    : {0}", glGetString( GL_VERSION ) );

        // Setup some general GL options
        glViewport( 0, 0, m_Properties.width, m_Properties.height );
        glEnable( GL_DEPTH_TEST );
        glClearColor( m_Properties.clearColor.x(), 
                      m_Properties.clearColor.y(),
                      m_Properties.clearColor.z(),
                      m_Properties.clearColor.w() );
    }

    CWindowEGL::~CWindowEGL()
    {
        eglDestroyContext( m_EGLDisplay, m_EGLContext );
        eglDestroySurface( m_EGLDisplay, m_EGLSurface );
        eglTerminate( m_EGLDisplay );
        gladLoaderUnloadEGL();
    }

    void CWindowEGL::_EnableCursorInternal()
    {
        // no "cursor" in headless mode
    }

    void CWindowEGL::_DisableCursorInternal()
    {
        // no "cursor" in headless mode
    }

    void CWindowEGL::_BeginInternal()
    {
        glClearColor( m_Properties.clearColor.x(), 
                      m_Properties.clearColor.y(),
                      m_Properties.clearColor.z(),
                      m_Properties.clearColor.w() );
        glClear( GL_COLOR_BUFFER_BIT |  GL_DEPTH_BUFFER_BIT );
    }

    void CWindowEGL::_EndInternal()
    {
        eglSwapBuffers( m_EGLDisplay, m_EGLSurface );
    }

    bool CWindowEGL::_ActiveInternal()
    {
        return true; // active as long as the program is active
    }

    void CWindowEGL::_RequestCloseInternal()
    {
        // nothing extra to setup here (will close when application closes)
    }
}