
#include <gl/CWindowEGL.h>

namespace engine
{
    CWindowEGL::CWindowEGL( const CWindowProps& properties )
        : CIWindow( properties )

    {
        m_type = eWindowType::HEADLESS_EGL;
        // Load EGL-functions using glad (first load without display)
        int eglVersion = gladLoaderLoadEGL( NULL );
        ENGINE_CORE_ASSERT( eglVersion, "CWindowEGL >>> something went wrong during first-load of EGL functions using GLAD (with no display)" );

        // Initialize EGL
        m_eglDisplay = eglGetDisplay( EGL_DEFAULT_DISPLAY );
        EGLint major, minor;
        ENGINE_CORE_ASSERT( eglInitialize( m_eglDisplay, &major, &minor ), "CWindowEGL >>> unable to initialize EGL" );
        ENGINE_CORE_TRACE( "CWindowEGL >>> initialized EGL display, major: {0}, minor: {1}", major, minor );

        // Reload EGL-functions using glad (now with a properly initliazed display)
        eglVersion = gladLoaderLoadEGL( m_eglDisplay );
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
            EGL_WIDTH, m_properties.width,
            EGL_HEIGHT, m_properties.height,
            EGL_NONE
        };

        eglChooseConfig( m_eglDisplay, configAttributes, &m_eglConfig, 1, &numConfigs );

        // Create an EGL surface
        m_eglSurface = eglCreatePbufferSurface( m_eglDisplay, m_eglConfig,
                                                pbufferAttributes );

        // Bind the API
        eglBindAPI( EGL_OPENGL_API );

        // Create a context and make it current
        m_eglContext = eglCreateContext( m_eglDisplay, // current display
                                         m_eglConfig, // current configuration
                                         EGL_NO_CONTEXT, // don't share context
                                         NULL ); // no extra attributes
        eglMakeCurrent( m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext );

        // Load gl-functions using glad
        ENGINE_CORE_ASSERT( gladLoadGL( (GLADloadfunc) eglGetProcAddress ), "CWindowEGL >>> failed to load GL using GLAD on the current EGL context" );
        ENGINE_CORE_INFO( "CWindowEGL >>> successfully initialized EGL window" );
        ENGINE_CORE_INFO( "OpenGL Info:" );
        ENGINE_CORE_INFO( "\tVendor     : {0}", glGetString( GL_VENDOR ) );
        ENGINE_CORE_INFO( "\tRenderer   : {0}", glGetString( GL_RENDERER ) );
        ENGINE_CORE_INFO( "\tVersion    : {0}", glGetString( GL_VERSION ) );

        // Setup some general GL options
        glViewport( 0, 0, m_properties.width, m_properties.height );
        glEnable( GL_DEPTH_TEST );
        glClearColor( m_properties.clearColor.x(), 
                      m_properties.clearColor.y(),
                      m_properties.clearColor.z(),
                      m_properties.clearColor.w() );
    }

    CWindowEGL::~CWindowEGL()
    {
        eglDestroyContext( m_eglDisplay, m_eglContext );
        eglDestroySurface( m_eglDisplay, m_eglSurface );
        eglTerminate( m_eglDisplay );
        gladLoaderUnloadEGL();
    }

    void CWindowEGL::_enableCursorInternal()
    {
        // no "cursor" in headless mode
    }

    void CWindowEGL::_disableCursorInternal()
    {
        // no "cursor" in headless mode
    }

    void CWindowEGL::_beginInternal()
    {
        // nothing extra to setup
    }

    void CWindowEGL::_endInternal()
    {
        eglSwapBuffers( m_eglDisplay, m_eglSurface );
    }

    bool CWindowEGL::_activeInternal()
    {
        return true; // active as long as the program is active
    }

    void CWindowEGL::_requestCloseInternal()
    {
        // nothing extra to setup here (will close when application closes)
    }

}