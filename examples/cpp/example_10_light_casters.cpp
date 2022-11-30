#include <array>
#include <glad/gl.h>

#include <renderer/window/window_t.hpp>
#include <renderer/input/input_manager_t.hpp>
#include <renderer/assets/shader_manager_t.hpp>
#include <renderer/camera/camera_t.hpp>
#include <renderer/geometry/geometry_t.hpp>
#include <renderer/geometry/geometry_factory.hpp>
#include <renderer/light/light_t.hpp>

#if defined(RENDERER_IMGUI)
#include <imgui.h>
#endif  // RENDERER_IMGUI

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

// NOLINTNEXTLINE
auto main() -> int {
    renderer::Window window(WINDOW_WIDTH, WINDOW_HEIGHT);
    renderer::ShaderManager shader_manager;
    renderer::InputManager input_manager;
    window.RegisterKeyboardCallback([&](int key, int action, int) {
        input_manager.CallbackKey(key, action);
    });

    auto program = shader_manager.LoadProgram(
        "basicPhong",
        renderer::EXAMPLES_PATH + "/resources/shaders/basicPhong_vert.glsl",
        renderer::EXAMPLES_PATH + "/resources/shaders/basicPhong_frag.glsl");

    auto geometry = renderer::CreateSphere(1.0F, 15, 15);

    renderer::Camera::uptr camera = std::make_unique<renderer::Camera>(
        Vec3(5.0F, 0.0F, 0.0F), Vec3(0.0F, 0.0F, 0.0F));

    renderer::Light::uptr light =
        std::make_unique<renderer::DirectionalLight>(Vec3(-1.0F, -1.0F, -1.0F));

    // Color of the global ambient light
    Vec3 ambient_light = {0.1F, 0.1F, 0.1F};

    // Color of the object to be rendered
    Vec3 object_color = {0.1F, 0.2F, 0.7F};

    while (window.active()) {
        window.Begin();

        if (input_manager.IsKeyDown(renderer::keys::KEY_ESCAPE)) {
            window.RequestClose();
        }

#if defined(RENDERER_IMGUI)
        ImGui::Begin("Options");
        // Allow the user to control the position of the camera
        static Vec3 s_camera_position = camera->position();
        ImGui::SliderFloat3("camera->position", s_camera_position.data(),
                            -10.0F, 10.0F);
        camera->SetPosition(s_camera_position);
        // Allow the user to set wireframe mode
        static bool s_wireframe = false;
        ImGui::Checkbox("wireframe", &s_wireframe);
        if (s_wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        // Allow the user to select which light caster to use
        std::array<const char*, 3> items_lights = {"directional", "point",
                                                   "spot"};
        {
            static size_t s_item_current_idx = 0;
            static bool s_change_item = false;
            const char* combo_preview_value =
                items_lights.at(s_item_current_idx);
            if (ImGui::BeginCombo("Lights", combo_preview_value)) {
                for (size_t i = 0; i < items_lights.size(); ++i) {
                    const auto IS_SELECTED = (s_item_current_idx == i);
                    if (ImGui::Selectable(items_lights.at(i), IS_SELECTED)) {
                        s_change_item = (s_item_current_idx != i);
                        s_item_current_idx = i;
                    }
                    if (IS_SELECTED) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            if (s_change_item) {
                switch (s_item_current_idx) {
                    case 0:  // directional
                        light = std::make_unique<renderer::DirectionalLight>(
                            Vec3(-1.0F, -1.0F, -1.0F));
                        break;
                    case 1:  // point
                        light = std::make_unique<renderer::PointLight>(
                            Vec3(5.0F, 5.0F, 5.0F));
                        break;
                    case 2:  // spot
                        light = std::make_unique<renderer::SpotLight>(
                            Vec3(5.0F, 5.0F, 5.0F), Vec3(0.0F, 0.0F, 0.0F));
                        break;
                    default:
                        break;
                }
                s_change_item = false;
            }
        }

        // Allow the user to select which geometry to visualize
        std::array<const char*, 7> items_geometries = {
            "plane",    "box",     "sphere", "ellipsoid",
            "cylinder", "capsule", "arrow"};
        {
            static size_t s_item_current_idx = 0;
            static bool s_change_item = false;
            const char* combo_preview_value =
                items_geometries.at(s_item_current_idx);
            if (ImGui::BeginCombo("Geometries", combo_preview_value)) {
                for (size_t i = 0; i < items_geometries.size(); ++i) {
                    const auto IS_SELECTED = (s_item_current_idx == i);
                    if (ImGui::Selectable(items_geometries.at(i),
                                          IS_SELECTED)) {
                        s_change_item = (s_item_current_idx != i);
                        s_item_current_idx = i;
                    }
                    if (IS_SELECTED) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
            if (s_change_item) {
                switch (s_item_current_idx) {
                    case 0:  // plane
                        geometry = renderer::CreatePlane(
                            1.0F, 1.0F, renderer::eAxis::AXIS_Z);
                        break;
                    case 1:  // box
                        geometry = renderer::CreateBox(1.0F, 1.0F, 1.0F);
                        break;
                    case 2:  // sphere
                        geometry = renderer::CreateSphere(1.0F, 20, 20);
                        break;
                    case 3:  // ellipsoid
                        geometry =
                            renderer::CreateEllipsoid(0.5F, 2.5F, 4.5F, 20, 20);
                        break;
                    case 4:  // cylinder
                        geometry = renderer::CreateCylinder(0.5F, 2.0F);
                        break;
                    case 5:  // capsule
                        geometry = renderer::CreateCapsule(0.5F, 2.0F);
                        break;
                    case 6:  // arrow
                        geometry = renderer::CreateArrow(0.5F);
                        break;
                    default:
                        break;
                }
                s_change_item = false;
            }
        }
        ImGui::End();
#endif  // RENDERER_IMGUI

        Mat4 model_matrix = Mat4::Identity();
        Mat4 normal_matrix = math::inverse(math::transpose(model_matrix));

        program->Bind();
        program->SetMat4("u_model_matrix", model_matrix);
        program->SetMat4("u_normal_matrix", normal_matrix);
        program->SetMat4("u_view_proj_matrix",
                         camera->proj_matrix() * camera->view_matrix());
        program->SetVec3("u_object_color", object_color);
        program->SetVec3("u_viewer_position", camera->position());
        program->SetVec3("u_ambient_light", ambient_light);

        // Configure directional light
        program->SetVec3("u_dir_light.direction", light->direction);
        program->SetVec3("u_dir_light.color", light->color);
        program->SetFloat("u_dir_light.intensity", light->intensity);

        geometry->VAO().Bind();

        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLint>(geometry->VAO().index_buffer().count()),
            GL_UNSIGNED_INT, nullptr);

#if defined(RENDERER_IMGUI)
        if (s_wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
#endif  // RENDERER_IMGUI

        geometry->VAO().Unbind();
        program->Unbind();
        window.End();
    }
}
