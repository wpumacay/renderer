
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

        m.def( "test_nparray_info", []( const py::array_t<float>& nparr )
            {
                auto info = nparr.request();
                std::cout << "size : " << info.size << std::endl;
                std::cout << "ndim : " << info.ndim << std::endl;
                for ( size_t i = 0; i < info.shape.size(); i++ )
                    std::cout << "shape[" << i << "] : " << info.shape[i] << std::endl;
                for ( size_t i = 0; i < info.strides.size(); i++ )
                    std::cout << "strides[" << i << "] : " << info.strides[i] << std::endl;
            } );
    }
}
