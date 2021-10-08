/// @file CWindowEGL.h
/// @brief Implementation of the window-class with EGL backend
#pragma once

#ifdef ENGINE_EGL_ENABLED

#include <glad/gl.h>
#include <glad/egl.h>
#include <window/CIWindow.h>

namespace engine
{
    /// \class CWindowEGL
    /// \brief Window class definition that uses EGL as windowing-backend
    class CWindowEGL : public CIWindow
    {
    public :

        /// \brief Constructs an EGL-backed window given user properties
        CWindowEGL( const CWindowProps& properties );

        /// \brief Releases the resources of this window
        ~CWindowEGL();

        /// \brief Returns a reference to the egl-config struct
        EGLConfig& eglConfig() { return m_EGLConfig; }

        /// \brief Returns an unmutable reference to the egl-config struct
        const EGLConfig& eglConfig() const { return m_EGLConfig; }

        /// \brief Returns a reference to the egl-display struct
        EGLDisplay& eglDisplay() { return m_EGLDisplay; }

        /// \brief Returns an unmutable reference to the egl-display struct
        const EGLDisplay& eglDisplay() const { return m_EGLDisplay; }

        /// \brief Returns a reference to the egl-surface struct
        EGLSurface& eglSurface() { return m_EGLSurface; }

        /// \brief Returns an unmutable reference to the egl-surface struct
        const EGLSurface& eglSurface() const { return m_EGLSurface; }

        /// \brief Returns a reference to the egl-context struct
        EGLContext& eglContext() { return m_EGLContext; }

        /// \brief Returns an unmutable reference to the egl-context struct
        const EGLContext& eglContext() const { return m_EGLContext; }

    protected :

        // Documentation inherited
        void _EnableCursorInternal() override;

        // Documentation inherited
        void _DisableCursorInternal() override;

        // Documentation inherited
        void _BeginInternal() override;

        // Documentation inherited
        void _EndInternal() override;

        // Documentation inherited
        bool _ActiveInternal() override;

        // Documentation inherited
        void _RequestCloseInternal() override;

    private :

        /// EGL-configuration struct
        EGLConfig m_EGLConfig;
        /// EGL-display struct
        EGLDisplay m_EGLDisplay;
        /// EGL-surface struct
        EGLSurface m_EGLSurface;
        /// EGL-context struct
        EGLContext m_EGLContext;

        ADD_CLASS_SMART_POINTERS(CWindowEGL);
    };
}

#endif /* ENGINE_EGL_ENABLED */
