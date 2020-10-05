
#include <common_py.h>
#include <shaders/CShader.h>
#include <shaders/CProgram.h>
#include <shaders/CShaderManager.h>

namespace py = pybind11;

namespace engine
{
    void bindings_shaders( py::module& m )
    {
        py::enum_<engine::eShaderType>( m, "ShaderType", py::arithmetic() )
            .value( "VERTEX", engine::eShaderType::VERTEX )
            .value( "FRAGMENT", engine::eShaderType::FRAGMENT );

        py::class_<CShader, CShader::ptr>( m, "Shader" )
            .def( py::init<const std::string&, const eShaderType&, const std::string&>() )
            .def_property_readonly( "name", CShader::name )
            .def_property_readonly( "type", CShader::type )
            .def_property_readonly( "source", CShader::source )
            .def_property_readonly( "opengl_id", CShader::opengl_id )
            .def_property_readonly( "compiled", CShader::compiled )
            .def( "__repr__", []( const CShader* self )
                {
                    std::string strrep;
                    strrep += "name: "


                    return strrep;
                } );
    }
}