
#include <common_py.h>

namespace py = pybind11;

namespace engine
{
    void bindings_common( py::module& m )
    {
        // Expose Axis enumerator
        py::enum_< engine::eAxis >( m, "Axis", py::arithmetic() )
            .value( "X", engine::eAxis::X )
            .value( "Y", engine::eAxis::Y )
            .value( "Z", engine::eAxis::Z );

        // Expose PixelDataType enumerator (used for texture dtype)
        py::enum_< engine::ePixelDataType >( m, "PixelDataType", py::arithmetic() )
            .value( "NONE", engine::ePixelDataType::NONE  )
            .value( "UINT_8", engine::ePixelDataType::UINT_8  )
            .value( "UINT_32", engine::ePixelDataType::UINT_32  )
            .value( "UINT_24_8", engine::ePixelDataType::UINT_24_8  )
            .value( "FLOAT_32", engine::ePixelDataType::FLOAT_32  );
    }
}