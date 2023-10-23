#include <renderer/camera/camera_controller_t.hpp>

namespace renderer {

auto ToString(const eCameraController& controller_type) -> std::string {
    switch (controller_type) {
        case eCameraController::NONE:
            return "none";
        case eCameraController::ORBIT:
            return "orbit";
        case eCameraController::FPS:
            return "fps";
    }
    return "undefined";
}

}  // namespace renderer
