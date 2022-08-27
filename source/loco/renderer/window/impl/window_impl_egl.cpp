
// clang-format off
#include <loco/utils/logging.hpp>
#include <loco/renderer/window/impl/window_impl_egl.hpp>
#include "glad/egl.h"
// clang-format on

namespace loco {
namespace renderer {

WindowImplEgl::WindowImplEgl(WindowProperties properties)
    : IWindowImpl(std::move(properties)) {
    auto egl_version = gladLoaderLoadEGL(nullptr);
    LOG_CORE_ASSERT(egl_version,
                    "WindowImplEgl >>> something went wrong during first load "
                    "of EGL functions using GLAD");

    m_Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);  // NOLINT
    EGLint gl_major{};
    EGLint gl_minor{};
    LOG_CORE_ASSERT(eglInitialize(m_Display, &gl_major, &gl_minor),
                    "WindowImplEgl >>> unable to initialize EGL");
    LOG_CORE_TRACE(
        "WindowImplEgl >>> initialized EGL display, major: {0}, minor: {1}",
        gl_major, gl_minor);

    egl_version = gladLoaderLoadEGL(m_Display);
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
        EGL_WIDTH, m_Properties.width,
        EGL_HEIGHT, m_Properties.height,
        EGL_NONE
    };
    // clang-format on

    eglChooseConfig(m_Display, static_cast<const EGLint*>(config_attribs),
                    &m_Config, 1, &num_config);

    m_Surface = eglCreatePbufferSurface(
        m_Display, m_Config, static_cast<const EGLint*>(pbuffer_attribs));

    eglBindAPI(EGL_OPENGL_API);

    m_Context = eglCreateContext(m_Display, m_Config, EGL_NO_CONTEXT, nullptr);

    eglMakeCurrent(m_Display, m_Surface, m_Surface, m_Context);

    // Load gl-functions using glad
    LOG_CORE_ASSERT(gladLoadGL(static_cast<GLADloadfunc>(eglGetProcAddress)),
                    "WindowImplEgl >>> failed to load GL using GLAD on the "
                    "current EGL context");
    LOG_CORE_INFO("WindowImplEgl >>> successfully initialized EGL window");
    LOG_CORE_INFO("OpenGL Info:");
    LOG_CORE_INFO("\tVendor     : {0}", glGetString(GL_VENDOR));
    LOG_CORE_INFO("\tRenderer   : {0}", glGetString(GL_RENDERER));
    LOG_CORE_INFO("\tVersion    : {0}", glGetString(GL_VERSION));

    // Setup some general GL options
    glViewport(0, 0, m_Properties.width, m_Properties.height);
    glEnable(GL_DEPTH_TEST);
    glClearColor(m_Properties.clear_color.x(), m_Properties.clear_color.y(),
                 m_Properties.clear_color.z(), m_Properties.clear_color.w());
}

WindowImplEgl::~WindowImplEgl() {
    if (m_Display != nullptr) {
        if (m_Context != nullptr) {
            eglDestroyContext(m_Display, m_Context);
            m_Context = nullptr;
        }
        if (m_Surface != nullptr) {
            eglDestroySurface(m_Display, m_Surface);
            m_Surface = nullptr;
        }
        eglTerminate(m_Display);
        m_Display = nullptr;
    }
    gladLoaderUnloadEGL();
}

auto WindowImplEgl::Begin() -> void {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

auto WindowImplEgl::End() -> void {
    if (m_Display != nullptr && m_Surface != nullptr) {
        eglSwapBuffers(m_Display, m_Surface);
    }
}

auto WindowImplEgl::RequestClose() -> void {
    m_Properties.dirty_request_close = true;
}

}  // namespace renderer
}  // namespace loco
