#include <renderer/material/material_t.hpp>

namespace renderer {

auto ToString(const eMaterialType& mat_type) -> std::string {
    switch (mat_type) {
        case eMaterialType::BASIC:
            return "Basic";
        case eMaterialType::LAMBERT:
            return "Lambert";
        case eMaterialType::PHONG:
            return "Phong";
        case eMaterialType::PBR:
            return "Pbr";
    }

    return "Undefined";
}

auto Material::toString() const -> std::string { return ""; }

}  // namespace renderer
