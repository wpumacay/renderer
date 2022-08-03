#include <loco/renderer/common.hpp>

namespace loco {
namespace renderer {

auto ToString(const ePixelDataType& dtype) -> std::string {
    switch (dtype) {
        case ePixelDataType::UINT_8:
            return "uint_8";
        case ePixelDataType::UINT_32:
            return "uint_32";
        case ePixelDataType::UINT_24_8:
            return "uint_24_8";
        case ePixelDataType::FLOAT_32:
            return "float_32";
        default: {
            // Reach a critical error here, as it shouldn't get here
        }
    }
}

auto operator<<(std::ostream& out_stream, const ePixelDataType& dtype)
    -> std::ostream& {
    auto dtype_str = ToString(dtype);
    out_stream << "[" << dtype_str <<"]";
    return out_stream;
}

}  // namespace renderer
}  // namespace loco
