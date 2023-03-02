
#include <glad/gl.h>

#include <utils/logging.hpp>
#include <renderer/window/impl/window_impl_glfw.hpp>
#include <spdlog/fmt/bundled/format.h>

#if defined(RENDERER_IMGUI)
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#endif  // RENDERER_IMGUI

namespace renderer {

auto GLFWwindowDeleter::operator()(GLFWwindow* ptr) const -> void {
    if (ptr != nullptr) {
        glfwDestroyWindow(ptr);
        glfwTerminate();
    }
}

WindowImplGlfw::WindowImplGlfw(WindowProperties properties)
    : IWindowImpl(std::move(properties)) {
    if (glfwInit() != GLFW_TRUE) {
        LOG_CORE_ERROR("There was an issue while initializing GLFW");
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_Properties.gl_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_Properties.gl_version_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    auto* glfw_window =
        glfwCreateWindow(m_Properties.width, m_Properties.height,
                         m_Properties.title.c_str(), nullptr, nullptr);
    if (glfw_window == nullptr) {
        LOG_CORE_ERROR("WindowImplGlfw >>> coudn't initialize GLFW window");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(glfw_window);
    LOG_CORE_ASSERT(gladLoadGL(glfwGetProcAddress),
                    "WindowImplGlfw >>> failed to load GL using GLAD on the "
                    "current context");
    LOG_CORE_INFO("OpenGL Info:");
    LOG_CORE_INFO("\tVendor     : {0}", fmt::ptr(glGetString(GL_VENDOR)));
    LOG_CORE_INFO("\tRenderer   : {0}", fmt::ptr(glGetString(GL_RENDERER)));
    LOG_CORE_INFO("\tVersion    : {0}", fmt::ptr(glGetString(GL_VERSION)));

    glfwSetInputMode(glfw_window, GLFW_STICKY_KEYS, GLFW_TRUE);
    int fbuffer_width = 0;
    int fbuffer_height = 0;
    glfwGetFramebufferSize(glfw_window, &fbuffer_width, &fbuffer_height);

    glViewport(0, 0, fbuffer_width, fbuffer_height);
    glEnable(GL_DEPTH_TEST);
    glClearColor(m_Properties.clear_color.x(), m_Properties.clear_color.y(),
                 m_Properties.clear_color.z(), m_Properties.clear_color.w());

    glfwSetWindowUserPointer(glfw_window, this);

    glfwSetKeyCallback(glfw_window, [](GLFWwindow* window_ptr, int key, int,
                                       int action, int mods) {
#if defined(RENDERER_IMGUI)
        ImGuiIO& imgui_io = ImGui::GetIO();
        if (imgui_io.WantCaptureKeyboard) {
            return;  // Let ImGui take control over
        }
#endif  // RENDERER_IMGUI
        auto* impl =
            static_cast<WindowImplGlfw*>(glfwGetWindowUserPointer(window_ptr));
        size_t num_callbacks = impl->m_ArrKeyboardCallbacksCount;
        auto& arr_callbacks = impl->m_ArrKeyboardCallbacks;
        for (size_t i = 0; i < num_callbacks; i++) {
            arr_callbacks.at(i)(key, action, mods);
        }
    });

    glfwSetMouseButtonCallback(glfw_window, [](GLFWwindow* window_ptr, int btn,
                                               int act, int) {
#if defined(RENDERER_IMGUI)
        ImGuiIO& imgui_io = ImGui::GetIO();
        if (imgui_io.WantCaptureMouse) {
            return;  // Let ImGui takes control over
        }
#endif  // RENDERER_IMGUI
        auto* impl =
            static_cast<WindowImplGlfw*>(glfwGetWindowUserPointer(window_ptr));
        size_t num_callbacks = impl->m_ArrMouseButtonCallbacksCount;
        auto& arr_callbacks = impl->m_ArrMouseButtonCallbacks;

        double x_cursor = 0;
        double y_cursor = 0;
        glfwGetCursorPos(window_ptr, &x_cursor, &y_cursor);
        for (size_t i = 0; i < num_callbacks; i++) {
            arr_callbacks.at(i)(btn, act, x_cursor, y_cursor);
        }
    });

    glfwSetCursorPosCallback(glfw_window, [](GLFWwindow* window_ptr,
                                             double xpos, double ypos) {
        auto* impl =
            static_cast<WindowImplGlfw*>(glfwGetWindowUserPointer(window_ptr));
        size_t num_callbacks = impl->m_ArrMouseMoveCallbacksCount;
        auto& arr_callbacks = impl->m_ArrMouseMoveCallbacks;

        for (size_t i = 0; i < num_callbacks; i++) {
            arr_callbacks.at(i)(xpos, ypos);
        }
    });

    glfwSetScrollCallback(glfw_window, [](GLFWwindow* window_ptr, double x_off,
                                          double y_off) {
        auto* impl =
            static_cast<WindowImplGlfw*>(glfwGetWindowUserPointer(window_ptr));
        size_t num_callbacks = impl->m_ArrScrollCallbacksCount;
        auto& arr_callbacks = impl->m_ArrScrollCallbacks;

        for (size_t i = 0; i < num_callbacks; i++) {
            arr_callbacks.at(i)(x_off, y_off);
        }
    });

    glfwSetFramebufferSizeCallback(glfw_window, [](GLFWwindow* window_ptr,
                                                   int width, int height) {
        auto* impl =
            static_cast<WindowImplGlfw*>(glfwGetWindowUserPointer(window_ptr));
        size_t num_callbacks = impl->m_ArrResizeCallbacksCount;
        auto& arr_callbacks = impl->m_ArrResizeCallbacks;

        for (size_t i = 0; i < num_callbacks; i++) {
            arr_callbacks.at(i)(width, height);
        }
    });

#if defined(RENDERER_IMGUI)
    // --------------------------------
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& imgui_io = ImGui::GetIO();
    (void)imgui_io;

    // --------------------------------
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // --------------------------------
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(glfw_window, true);
    ImGui_ImplOpenGL3_Init("#version 130");
#endif  // RENDERER_IMGUI

    // Keep ownership of the glfw-window
    m_GlfwWindow = std::unique_ptr<GLFWwindow, GLFWwindowDeleter>(glfw_window);
}

WindowImplGlfw::~WindowImplGlfw() {
    m_GlfwWindow = nullptr;
#if defined(RENDERER_IMGUI)
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
#endif  // RENDERER_IMGUI
}

auto WindowImplGlfw::RegisterKeyboardCallback(const KeyboardCallback& callback)
    -> void {
    m_ArrKeyboardCallbacks.at(m_ArrKeyboardCallbacksCount++) = callback;
}

auto WindowImplGlfw::RegisterMouseButtonCallback(
    const MouseButtonCallback& callback) -> void {
    m_ArrMouseButtonCallbacks.at(m_ArrMouseButtonCallbacksCount++) = callback;
}

auto WindowImplGlfw::RegisterMouseMoveCallback(
    const MouseMoveCallback& callback) -> void {
    m_ArrMouseMoveCallbacks.at(m_ArrMouseMoveCallbacksCount++) = callback;
}

auto WindowImplGlfw::RegisterScrollCallback(const ScrollCallback& callback)
    -> void {
    m_ArrScrollCallbacks.at(m_ArrScrollCallbacksCount++) = callback;
}

auto WindowImplGlfw::RegisterResizeCallback(const ResizeCallback& callback)
    -> void {
    m_ArrResizeCallbacks.at(m_ArrResizeCallbacksCount++) = callback;
}

auto WindowImplGlfw::EnableCursor() -> void {
    if (m_GlfwWindow != nullptr) {
        glfwSetInputMode(m_GlfwWindow.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

auto WindowImplGlfw::DisableCursor() -> void {
    if (m_GlfwWindow != nullptr) {
        glfwSetInputMode(m_GlfwWindow.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

auto WindowImplGlfw::Begin() -> void {
    glClearColor(m_Properties.clear_color.x(), m_Properties.clear_color.y(),
                 m_Properties.clear_color.z(), m_Properties.clear_color.w());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();

#if defined(RENDERER_IMGUI)
    // --------------------------------
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
#endif  // RENDERER_IMGUI
}

auto WindowImplGlfw::End() -> void {
    if (m_GlfwWindow != nullptr) {
#if defined(RENDERER_IMGUI)
        // Draw all data generated by ImGui calls
        ImGui::Render();
        // Render all ui-elements
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif  // RENDERER_IMGUI
        glfwSwapBuffers(m_GlfwWindow.get());
    }
}

auto WindowImplGlfw::RequestClose() -> void {
    if (m_GlfwWindow != nullptr) {
        glfwSetWindowShouldClose(m_GlfwWindow.get(), GLFW_TRUE);
    }
}

}  // namespace renderer
