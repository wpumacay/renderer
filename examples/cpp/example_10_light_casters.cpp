#include <array>
#include <glad/gl.h>

#include <renderer/window/window_t.hpp>
#include <renderer/input/input_manager_t.hpp>
#include <renderer/assets/shader_manager_t.hpp>
#include <renderer/camera/camera_t.hpp>
#include <renderer/geometry/geometry_t.hpp>
#include <renderer/geometry/geometry_factory.hpp>
#include <renderer/light/light_t.hpp>

#include <utils/logging.hpp>
#include "renderer/common.hpp"

#if defined(RENDERER_IMGUI)
#include <imgui.h>
#endif  // RENDERER_IMGUI

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 600;

// NOLINTNEXTLINE
#define IMGUI_COMBO(combo_items, combo_title, combo_callback)                 \
    {                                                                         \
        static size_t s_item_current_idx = 0;                                 \
        static bool s_change_item = false;                                    \
        const char* combo_preview_value = combo_items.at(s_item_current_idx); \
        if (ImGui::BeginCombo(combo_title, combo_preview_value)) {            \
            for (size_t i = 0; i < combo_items.size(); ++i) {                 \
                const auto IS_SELECTED = (s_item_current_idx == i);           \
                if (ImGui::Selectable(combo_items.at(i), IS_SELECTED)) {      \
                    s_change_item = (s_item_current_idx != i);                \
                    s_item_current_idx = i;                                   \
                }                                                             \
                if (IS_SELECTED) {                                            \
                    ImGui::SetItemDefaultFocus();                             \
                }                                                             \
            }                                                                 \
            ImGui::EndCombo();                                                \
        }                                                                     \
        if (s_change_item) {                                                  \
            combo_callback(s_item_current_idx);                               \
            s_change_item = false;                                            \
        }                                                                     \
    }

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

    auto geometry = renderer::CreatePlane(5.0F, 5.0F, renderer::eAxis::AXIS_Z);

    renderer::Camera::ptr camera = std::make_shared<renderer::Camera>(
        Vec3(5.0F, 5.0F, 5.0F), Vec3(0.0F, 0.0F, 0.0F));

    renderer::Light::ptr light =
        std::make_shared<renderer::DirectionalLight>(Vec3(-1.0F, -2.0F, -3.0F));

    // Color of the global ambient light
    Vec3 ambient_light = {0.1F, 0.1F, 0.1F};

    // Color of the object to be rendered
    Vec3 object_color = {1.0F, 0.5F, 0.31F};

    while (window.active()) {
        window.Begin();

        if (input_manager.IsKeyDown(renderer::keys::KEY_ESCAPE)) {
            window.RequestClose();
        }

#if defined(RENDERER_IMGUI)
        ImGui::Begin("Options");
        // Allow the user to control the position of the camera
        static Vec3 s_camera_position = camera->position();
        ImGui::SliderFloat3("Camera.position", s_camera_position.data(), -10.0F,
                            10.0F);
        camera->SetPosition(s_camera_position);
        // Allow the user to set wireframe mode
        static bool s_wireframe = false;
        ImGui::Checkbox("Wireframe", &s_wireframe);
        if (s_wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }

        ImGui::SliderFloat3("Ambient light", ambient_light.data(), 0.0F, 1.0F);

        // Allow the user to select which light caster to use
        std::array<const char*, 3> items_lights = {"directional", "point",
                                                   "spot"};
        IMGUI_COMBO(
            items_lights, "Light.type", [&](size_t combo_index) -> void {
                switch (combo_index) {
                    case 0:  // directional
                        light = std::make_shared<renderer::DirectionalLight>(
                            Vec3(-1.0F, -2.0F, -3.0F));
                        break;
                    case 1:  // point
                        light = std::make_shared<renderer::PointLight>(
                            Vec3(3.0F, 4.0F, 5.0F));
                        break;
                    case 2:  // spot
                        light = std::make_shared<renderer::SpotLight>(
                            Vec3(0.0F, 0.0F, 5.0F), Vec3(0.0F, 0.0F, -1.0F));
                        break;
                    default:
                        break;
                }
                LOG_INFO("Using light type: {0}",
                         renderer::ToString(light->type));
            });

        // Light properties
        ImGui::ColorEdit3("Light.color", light->color.data());
        ImGui::SliderFloat("Light.intensity", &light->intensity, 0.0F, 1.0F);
        switch (light->type) {
            case renderer::eLightType::DIRECTIONAL: {
                auto dir_light =
                    std::dynamic_pointer_cast<renderer::DirectionalLight>(
                        light);
                break;
            }
            case renderer::eLightType::POINT: {
                auto point_light =
                    std::dynamic_pointer_cast<renderer::PointLight>(light);
                break;
            }
            case renderer::eLightType::SPOT: {
                auto spot_light =
                    std::dynamic_pointer_cast<renderer::SpotLight>(light);
                ImGui::SliderFloat("Light.innerCutoff",
                                   &light->innerCutoffAngle, PI / 9.0F,
                                   PI / 3.0F);
                ImGui::SliderFloat("Light.outerCutoff",
                                   &light->outerCutoffAngle,
                                   light->innerCutoffAngle, PI / 3.0F);
                break;
            }
        }

        // Allow the user to select which geometry to visualize
        std::array<const char*, 7> items_geometries = {
            "plane",    "box",     "sphere", "ellipsoid",
            "cylinder", "capsule", "arrow"};
        IMGUI_COMBO(
            items_geometries, "Geometry", [&](size_t combo_index) -> void {
                switch (combo_index) {
                    case 0:  // plane
                        geometry = renderer::CreatePlane(
                            5.0F, 5.0F, renderer::eAxis::AXIS_Z);
                        break;
                    case 1:  // box
                        geometry = renderer::CreateBox(1.0F, 1.0F, 1.0F);
                        break;
                    case 2:  // sphere
                        geometry = renderer::CreateSphere(1.0F, 30, 30);
                        break;
                    case 3:  // ellipsoid
                        geometry =
                            renderer::CreateEllipsoid(0.5F, 1.0F, 1.5F, 30, 30);
                        break;
                    case 4:  // cylinder
                        geometry = renderer::CreateCylinder(0.5F, 1.0F);
                        break;
                    case 5:  // capsule
                        geometry = renderer::CreateCapsule(0.5F, 1.0F);
                        break;
                    case 6:  // arrow
                        geometry = renderer::CreateArrow(0.5F);
                        break;
                    default:
                        break;
                }
            });

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

        if (auto dir_light =
                std::dynamic_pointer_cast<renderer::DirectionalLight>(light)) {
            // Configure directional light
            program->SetVec3("u_dir_light.direction", light->direction);
            program->SetVec3("u_dir_light.color", light->color);
            program->SetFloat("u_dir_light.intensity", light->intensity);
            program->SetInt("u_dir_light.enabled", GL_TRUE);
            program->SetInt("u_point_light.enabled", GL_FALSE);
            program->SetInt("u_spot_light.enabled", GL_FALSE);
        } else if (auto point_light =
                       std::dynamic_pointer_cast<renderer::PointLight>(light)) {
            // Configure point light
            program->SetVec3("u_point_light.position", light->position);
            program->SetVec3("u_point_light.color", light->color);
            program->SetFloat("u_point_light.intensity", light->intensity);
            program->SetInt("u_point_light.enabled", GL_TRUE);
            program->SetInt("u_dir_light.enabled", GL_FALSE);
            program->SetInt("u_spot_light.enabled", GL_FALSE);
        } else if (auto spot_light =
                       std::dynamic_pointer_cast<renderer::SpotLight>(light)) {
            // Configure spot light
            program->SetVec3("u_spot_light.position", light->position);
            program->SetVec3("u_spot_light.direction", light->direction);
            program->SetFloat("u_spot_light.innerCutOffCos",
                              std::cos(light->innerCutoffAngle));
            program->SetFloat("u_spot_light.outerCutOffCos",
                              std::cos(light->outerCutoffAngle));
            program->SetVec3("u_spot_light.color", light->color);
            program->SetFloat("u_spot_light.intensity", light->intensity);
            program->SetInt("u_spot_light.enabled", GL_TRUE);
            program->SetInt("u_dir_light.enabled", GL_FALSE);
            program->SetInt("u_point_light.enabled", GL_FALSE);
        }

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
