#pragma once

#include <glad/egl.h>
#include <gl/CIWindow.h>

namespace engine
{
    class CWindowEGL : public CIWindow
    {
    public :

        CWindowEGL( const CWindowProps& properties );
        ~CWindowEGL();

        EGLConfig& eglConfig() { return m_eglConfig; }
        EGLDisplay& eglDisplay() { return m_eglDisplay; }
        EGLSurface& eglSurface() { return m_eglSurface; }
        EGLContext& eglContext() { return m_eglContext; }

        const EGLConfig& eglConfig() const { return m_eglConfig; }
        const EGLDisplay& eglDisplay() const { return m_eglDisplay; }
        const EGLSurface& eglSurface() const { return m_eglSurface; }
        const EGLContext& eglContext() const { return m_eglContext; }

    protected :

        void _enableCursorInternal() override;
        void _disableCursorInternal() override;
        void _beginInternal() override;
        void _endInternal() override;
        bool _activeInternal() override;
        void _requestCloseInternal() override;

    private :

        EGLConfig  m_eglConfig;
        EGLDisplay m_eglDisplay;
        EGLSurface m_eglSurface;
        EGLContext m_eglContext;
    };
}