#include <utils/logging.hpp>
#include <renderer/window/impl/window_impl_egl.hpp>
#include <spdlog/fmt/bundled/format.h>

namespace renderer {

WindowImplEgl::WindowImplEgl(WindowConfig config)
    : IWindowImpl(std::move(config)) {
    auto egl_version = gladLoaderLoadEGL(nullptr);
    LOG_CORE_ASSERT(egl_version,
                    "WindowImplEgl >>> something went wrong during first load "
                    "of EGL functions using GLAD");

    m_EglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);  // NOLINT
    EGLint gl_major{};
    EGLint gl_minor{};
    LOG_CORE_ASSERT(eglInitialize(m_EglDisplay, &gl_major, &gl_minor),
                    "WindowImplEgl >>> unable to initialize EGL");
    LOG_CORE_TRACE(
        "WindowImplEgl >>> initialized EGL display, major: {0}, minor: {1}",
        gl_major, gl_minor);

    egl_version = gladLoaderLoadEGL(m_EglDisplay);
    LOG_CORE_ASSERT(
        egl_version,
        "WindowImplEgl >>> something went wrong while loading EGL using GLAD");
    LOG_CORE_TRACE("WindowImplEgl >>> initialized EGL version: {0}",
                   egl_version);

    EGLint num_config{};
    // clang-format off
    // NOLINTNEXTLINE
    const EGLint config_attribs[] = {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_DEPTH_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_NONE
    };
    // NOLINTNEXTLINE
    const EGLint pbuffer_attribs[] = {
        EGL_WIDTH, m_Config.width,
        EGL_HEIGHT, m_Config.height,
        EGL_NONE
    };
    // clang-format on

    eglChooseConfig(m_EglDisplay, static_cast<const EGLint*>(config_attribs),
                    &m_EglConfig, 1, &num_config);

    m_EglSurface = eglCreatePbufferSurface(
        m_EglDisplay, m_EglConfig, static_cast<const EGLint*>(pbuffer_attribs));

    eglBindAPI(EGL_OPENGL_API);

    m_EglContext =
        eglCreateContext(m_EglDisplay, m_EglConfig, EGL_NO_CONTEXT, nullptr);

    eglMakeCurrent(m_EglDisplay, m_EglSurface, m_EglSurface, m_EglContext);

    // Load gl-functions using glad
    LOG_CORE_ASSERT(gladLoadGL(eglGetProcAddress),
                    "WindowImplEgl >>> failed to load GL using GLAD on the "
                    "current EGL context");
    LOG_CORE_INFO("WindowImplEgl >>> successfully initialized EGL window");
    LOG_CORE_INFO("OpenGL Info:");
    LOG_CORE_INFO("\tVendor     : {0}", fmt::ptr(glGetString(GL_VENDOR)));
    LOG_CORE_INFO("\tRenderer   : {0}", fmt::ptr(glGetString(GL_RENDERER)));
    LOG_CORE_INFO("\tVersion    : {0}", fmt::ptr(glGetString(GL_VERSION)));

    // Setup some general GL options
    glViewport(0, 0, m_Config.width, m_Config.height);
    glEnable(GL_DEPTH_TEST);
    glClearColor(m_Config.clear_color.x(), m_Config.clear_color.y(),
                 m_Config.clear_color.z(), m_Config.clear_color.w());
}

WindowImplEgl::~WindowImplEgl() {
    if (m_EglDisplay != nullptr) {
        if (m_EglContext != nullptr) {
            eglDestroyContext(m_EglDisplay, m_EglContext);
            m_EglContext = nullptr;
        }
        if (m_EglSurface != nullptr) {
            eglDestroySurface(m_EglDisplay, m_EglSurface);
            m_EglSurface = nullptr;
        }
        eglTerminate(m_EglDisplay);
        m_EglDisplay = nullptr;
    }
    gladLoaderUnloadEGL();
}

auto WindowImplEgl::Begin() -> void {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

auto WindowImplEgl::End() -> void {
    if (m_EglDisplay != nullptr && m_EglSurface != nullptr) {
        eglSwapBuffers(m_EglDisplay, m_EglSurface);
    }
}

auto WindowImplEgl::SetClearColor(const Vec4& color) -> void {
    glClearColor(color.x(), color.y(), color.z(), color.w());
}

}  // namespace renderer
