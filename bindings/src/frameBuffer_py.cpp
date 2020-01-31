
#include <frameBuffer_py.h>

namespace py = pybind11;

namespace engine
{
    void bindings_frameBuffer( py::module& m )
    {
        py::enum_< engine::eFboAttachment >( m, "FboAttachment", py::arithmetic() )
            .value( "NONE", engine::eFboAttachment::NONE )
            .value( "COLOR", engine::eFboAttachment::COLOR )
            .value( "DEPTH", engine::eFboAttachment::DEPTH )
            .value( "STENCIL", engine::eFboAttachment::STENCIL )
            .value( "DEPTH_STENCIL", engine::eFboAttachment::DEPTH_STENCIL );

        py::class_< CAttachmentConfig >( m, "AttachmentConfig" )
            .def( py::init<>() )
            .def_readwrite( "name", &CAttachmentConfig::name )
            .def_readwrite( "attachment", &CAttachmentConfig::attachment )
            .def_readwrite( "width", &CAttachmentConfig::width )
            .def_readwrite( "height", &CAttachmentConfig::height )
            .def_readwrite( "texInternalFormat", &CAttachmentConfig::texInternalFormat )
            .def_readwrite( "texFormat", &CAttachmentConfig::texFormat )
            .def_readwrite( "texPixelDataType", &CAttachmentConfig::texPixelDataType )
            .def_readwrite( "texWrapU", &CAttachmentConfig::texWrapU )
            .def_readwrite( "texWrapV", &CAttachmentConfig::texWrapV )
            .def_property( "texBorderColorU",
                           []( const CAttachmentConfig* self ) -> py::array_t<float32>
                                {
                                    return tinymath::vector_to_nparray<float32,4>( self->texBorderColorU );
                                },
                           []( CAttachmentConfig* self, const py::array_t<float32>& arr_texBorderColorU )
                                {
                                    self->texBorderColorU = tinymath::nparray_to_vector<float32,4>( arr_texBorderColorU );
                                } )
            .def_property( "texBorderColorV",
                           []( const CAttachmentConfig* self ) -> py::array_t<float32>
                                {
                                    return tinymath::vector_to_nparray<float32,4>( self->texBorderColorV );
                                },
                           []( CAttachmentConfig* self, const py::array_t<float32>& arr_texBorderColorV )
                                {
                                    self->texBorderColorV = tinymath::nparray_to_vector<float32,4>( arr_texBorderColorV );
                                } )
            .def( "__repr__", []( const CAttachmentConfig* self )
                {
                    auto _strrep = std::string( "AttachmentConfig(\n" );
                    _strrep += "cpp-address : " + engine::pointerToHexAddress( self ) + "\n";
                    _strrep += "name        : " + self->name + "\n";
                    _strrep += "width       : " + std::to_string( self->width ) + "\n";
                    _strrep += "height      : " + std::to_string( self->height ) + "\n";
                    _strrep += "texIntFormat: " + engine::toString( self->texInternalFormat ) + "\n";
                    _strrep += "texFormat   : " + engine::toString( self->texFormat ) + "\n";
                    _strrep += "pixelType   : " + engine::toString( self->texPixelDataType ) + "\n";
                    _strrep += "texWrapU    : " + engine::toString( self->texWrapU ) + "\n";
                    _strrep += "texWrapV    : " + engine::toString( self->texWrapV ) + "\n";
                    _strrep += "texBrdColorU: " + engine::toString( self->texBorderColorU ) + "\n";
                    _strrep += "texBrdColorV: " + engine::toString( self->texBorderColorV ) + "\n";
                    return _strrep;
                } );

        py::class_< CFrameBuffer >( m, "FrameBuffer" )
            .def( py::init<>() )
            .def( "addAttachment", &CFrameBuffer::addAttachment, py::arg( "config" ) )
            .def( "bind", &CFrameBuffer::bind )
            .def( "unbind", &CFrameBuffer::unbind )
            .def( "resize", &CFrameBuffer::resize, py::arg( "width" ), py::arg( "height" ) )
            .def( "getTextureAttachment", 
                  &CFrameBuffer::getTextureAttachment, 
                  py::arg( "name" ), py::return_value_policy::reference )
            .def( "getConfigAttachment",
                  &CFrameBuffer::getConfigAttachment,
                  py::arg( "name" ), py::return_value_policy::reference )
            .def( "textures", &CFrameBuffer::textures, py::return_value_policy::reference )
            .def( "configs", &CFrameBuffer::configs, py::return_value_policy::reference )
            .def_property_readonly( "width", &CFrameBuffer::width )
            .def_property_readonly( "height", &CFrameBuffer::height )
            .def_property_readonly( "openglId", &CFrameBuffer::openglId )
            .def( "__repr__", []( const CFrameBuffer* self )
                {
                    auto _strrep = std::string( "FrameBuffer(\n" );
                    _strrep += "cpp-address : " + engine::pointerToHexAddress( self ) + "\n";
                    _strrep += "width       : " + std::to_string( self->width() ) + "\n";
                    _strrep += "height      : " + std::to_string( self->height() ) + "\n";
                    _strrep += "openglId    : " + std::to_string( self->openglId() ) + "\n";
                    return _strrep;
                } );
    }
}